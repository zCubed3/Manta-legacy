#!/bin/sh

# Validates all the shaders within projects/engine_data

# https://unix.stackexchange.com/questions/432052/shellcheck-warns-about-loops-over-find-output-even-when-given-the-path-to-begin
find ./projects/engine_data/ -type f -name "*.glsl" -exec ./scripts/ValidateShader.sh {} \;