#!/bin/bash

# Define the path to the riscv-sim program
RISCV_SIM_PATH="./riscv-sim"

INST_FILE="./proj2_8_inst.bin"
DATA_FILE="./proj2_8_data.bin"


# Define the path to the output file
OUTPUT_FILE="my_output.txt"

# Check if the riscv-sim program exists
if [ ! -x "$RISCV_SIM_PATH" ]; then
  echo "Error: riscv-sim program not found or not executable."
  exit 1
fi

# Check if the proj1_1.bin file exists
if [ ! -f "$INST_FILE" ]; then
  echo "Error: proj2.bin file not found."
  exit 1
fi

# Run the riscv-sim program with the specified command and redirect the output to OUTPUT_FILE and console
"$RISCV_SIM_PATH" "$INST_FILE" "$DATA_FILE" 2001 2>&1 | tee "$OUTPUT_FILE"

# Check if the riscv-sim program ran successfully
if [ $? -eq 0 ]; then
  echo "riscv-sim executed successfully. Check $OUTPUT_FILE for output."
else
  echo "Error: riscv-sim encountered an error. Check $OUTPUT_FILE for details."
fi

# Exit the script
exit 0
