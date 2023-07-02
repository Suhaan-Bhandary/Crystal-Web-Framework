# HTML Template

The html template will take json as data and file path as html template. Json object is passed.

## Process

- [x] HTML File is first read
- [ ] HTML File is then compiled
  - [ ] First Define the syntax
  - [ ] Replace the Template with html by compiling

## Syntax

### Simple Block

- Code Block to handle variable and equation

```cpp
    {{  }}
```

- Printing value

```cpp
    {{ variable }}
    {{ variable1 + variable2 }}
```

### Action blocks

- For Loop

```cpp
    {{ % for num in array % }}
        // HTML Code with nested things
        {{ num }}
    {{ % endFor % }}
```

```cpp
    {{ % for num in range(0, 5) % }}
        // HTML Code with nested things
        {{ num }}
    {{ % endFor % }}
```

- If else

```cpp
    {{ % if (condition) % }}
        // HTML Code with nested things
        {{ variable }}
    {{ % else % }}
        {{ variable }}
    {{ % endIf % }}
```
