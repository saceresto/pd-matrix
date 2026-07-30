# pd-matrix

## Goal
This project provides C externals for Pure Data to process matrices. It uses a flat list format for matrix data.

## Matrix Format
A matrix list uses this order:
[rows, cols, value_0, value_1, ..., value_N-1]

- Index 0 stores the row count.
- Index 1 stores the column count.
- Index 2 and higher store the data values.

## Objects
- mtx_create: Creates a matrix list from raw input values.
- mtx_random: Generates a matrix list filled with random values.
- mtx_identity: Generates an identity matrix list.
- mtx_store: Stores a matrix list in memory.
- mtx_print: Displays a matrix grid in the Pure Data console.
- mtx_add: Adds two matrix lists together.
- mtx_transpose: Transposes a matrix list.

## System Features
- Header Validation: Verifies matrix dimensions before processing data.
- Error Propagation: Sends an error bang out of a secondary outlet when dimension validation fails.

## Future
discrete differential operators and graph Laplacians...
