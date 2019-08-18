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


# Approaches

## Naïve approach

1. Given a `fr(R)` and an `M`, generate every possible `P` from `fr(R)`
1. For each `P`, check if it is valid and calculate its score
1. Sort valid `P`s by their score
