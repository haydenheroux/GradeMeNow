#include <iostream>
#include <unordered_map>

namespace grader {
struct grade {
  float points, maximum_points;

  grade() : points(0), maximum_points(0) {}

  grade(float points, float maximum_points)
      : points(points), maximum_points(maximum_points) {}

  /* Add this grade to another grade. */
  void accumulate_into(grade *accumulator) const {
    accumulator->points += this->points;
    accumulator->maximum_points += this->maximum_points;
  }

  /* Create a new grade that is out of `amount` maximum points. */
  grade scaled_to(float new_maximum) const {
    if (this->points == 0 || this->maximum_points == 0) {
      return grade{0, new_maximum};
    }

    return grade{this->points * new_maximum / this->maximum_points,
                 new_maximum};
  }
};

typedef unsigned int category;
typedef std::unordered_multimap<category, grade> categorized_grades;
typedef std::unordered_map<category, float> weights;

grade total(categorized_grades grades, weights weights) {
  std::unordered_map<category, grade> accumulators;

  for (const auto &[category, grade] : grades) {
    accumulators.try_emplace(category, 0, 0);
    auto accumulator = &accumulators.at(category);
    grade.accumulate_into(accumulator);
  }

  grade result;

  for (const auto &[category, accumulated] : accumulators) {
    auto weight = weights.at(category);
    accumulated.scaled_to(weight).accumulate_into(&result);
  }

  return result;
}

}; // namespace grader

int main() {
  grader::categorized_grades grades{};
  grader::weights weights{};

  // 0 = homework, 1 = exam, 2 = final
  weights.emplace(0, 0.25);
  weights.emplace(1, 0.45);
  weights.emplace(2, 0.30);

  grades.emplace(0, grader::grade{10, 10});
  grades.emplace(0, grader::grade{10, 10});
  grades.emplace(0, grader::grade{10, 10});
  grades.emplace(0, grader::grade{5, 10});
  grades.emplace(0, grader::grade{5, 10});

  grades.emplace(1, grader::grade{90, 100});
  grades.emplace(1, grader::grade{85, 100});
  grades.emplace(1, grader::grade{95, 100});

  grades.emplace(2, grader::grade{95, 100});

  grader::grade final_grade = grader::total(grades, weights);

  std::cout << final_grade.points << " / " << final_grade.maximum_points
            << std::endl;

  return 0;
}
