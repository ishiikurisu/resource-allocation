# Resource Allocation

## Problem Description

We have many _resources_, each one:

- belonging to a _category_
- having a _score_
- costing a _price_

With these resources, we can build _plans_, which are a subset of these resources. My plan's score is the sum of the score of my resources, while the price of my plan is the sum of the cost of each of my resources.

We also have restrictions:

- I have to select a specific number of resources for my plan
- I have to select a specific number of resources for each category
- My plan has to cost at most a given value

**Goal**: Given these restrictions and a list of resources, build a plan that maximizes the score.
