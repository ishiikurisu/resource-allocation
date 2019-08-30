# Description

## Mathematical description

Let a resource `R` be a tuple:

```
R := (c, s, p)
```

where:

- `c` is the category of this resource
- `s in RR` is the score of this resource
- `p in RR` is the price of this resource

`c` should be one of the elements of `fr C`, the set of all possible categories.

We will denote `c(R)` as a function that indicates the category of a resource `R`. The same will be valid for the other properties of the resource.

Let `fr R` be the set of every resource available.

Let a plan `P := {R_i}_(i=1)^(N)` be a subset of `fr R`.

Let a restriction be a statement over `P`. A plan is said to be _valid_ iff, given a list of restrictions `fr(L)`, the following happens:

```
AA L in fr(L) | L(P) = TT
```

Each one of the restrictions of the description can be mapped to a different function in `fr(L)`. For instance:

- `AA C in fr(C) | sum_(R)^(P) #{r | c(r) = C} = Q(C) | Q : fr(C) -> ZZ`
- `#P = T : T in ZZ`
- `sum_(R)^(P) p(R) < W : W in RR`

For convenience, we will define `M := (Q, T, W)`

The goal is to discover, from the universe `fr(P)` of every possible plans, a `P` that:

- satisfies every given condition
- maximizes the score function


## Data description

Since I want to write programs that solve this problem, I will define some file formats for each of the inputs.

From the mathematical description, I believe I only need to define `fr(R)` and `M`.

`fr(R)` can be a table with 3 columns: `c`, `s`,`p`. A JSON lines file or a JSON records one should be enough to cover this.

`M` is a class with 3 properties: `Q`, `T`, and `W`. `T` and `W` are both numbers, so no problem. `Q` looks like a dictionary where each key is a `C in fr(C)` and each value, its `Q(C)`. It is important that this dictionary covers every `C in fr(C)`. This can be modelled with a JSON file.

`fr_R.json`:

```
[
    {
        "c": "A",
        "s": 123,
        "p": 50
    }, {
        "c": "B",
        "s": 456,
        "p": 70
    }, {
        "c": "B",
        "s": 789,
        "p": 90
    }
]
```

`M.json`:

```
{
    "Q": {
        "A": 1,
        "B": 1
    },
    "T": 2,
    "M": 1000
}
```

One thing that can be improved is that `T` can be derived from `Q`:

```
T = sum_(C in Q) Q(C)
```


# Approaches

## Naïve approach

1. Given a `fr(R)` and an `M`, generate every possible `P` from `fr(R)`
1. For each `P`, check if it is valid and calculate its score
1. Sort valid `P`s by their score

This naïve should be careful to not consume all memory available when generating every possible `P` from `fr(R)`. This means processing each `P` as it is generated instead of generating all `P`s at once. Also, we only need the `P : #P = T`.


## Less naïve approach

1. Separate all resources into their own categories
1. For each category, apply the naïve algorithm in parallel
1. Incorporate all results into a single one

Another idea to improve this is to use the category knowledge in our favor to execute this in parallel and probably save some time. It is safe to assume that the best score for a plan will be the sum of the best score of the best plan for each category. By separating the categories, it is possible to execute the naïve algorithm a lot faster.

The problem of this implementation is that it doesn't take the price of the resources into consideration. Some ways of ignoring this is:

- Considering that each category should cost at most a proportion of the total cost, probably `Q/T`.
- Outputting the best for each category and then applying the naíve approach once more.


## Backtracking

The idea of this approach is demonstrated in the following pseudocode:

``` coffee
p = {}   # set of all players
f = (t, i) ->  # t: team, i: index of player
  if i > p.length
    s(t)
  else
    max(f(t << p[i], i+1),
        f(t        , i+1))

# score of a team
s = t ->
  # TODO use memoization here
  if is_invalid(t)  # due to money or bad formation
    -Infinity
  else
    calculate_score(t)

console.log f([], 0)
```

If `n` is the size of `p`, then this algorithm has time complexity in the worst case scenario equals to:

```
O = sum_(i=1)^(n) 2^i
```

which it still isn't practible


## Dynamic programming

DP is backtracking plus memoization. How can I memoize this algorithm? I could iterate over a matrix "i-th choice" * "j-th player". This means that, if we have a matrix `S` for scores, then

``` py
S[j][i] = max(
    score(fr_r[j]) + S[j+1][i+1],  
    S[j][i+1],
)
```

This is the same as writting:

``` py
S[j][i] = max(
    f(j) + S[j-1][i-1],
    S[j][i-1],
)
```

- [ ] Calculate best possible score for my money

By keeping track of a score matrix and a money matrix, I can determine what is the best score for my money. By determining what is the best score, _maybe_ I can reconstruct the best team. How?

- [ ] Determine team that obtained a given score in the matrix

Take a look at the `T`-th row: from that row, we have all teams with the size we need. By selecting only this row, we can focus on the teams that really matters and start from there.


## Other approaches

- Operations research?
