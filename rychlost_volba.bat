@echo off

set /p r="rychlost (1-5) ? "
title text to morse, rychlost %r%

text_to_morse_11.exe %r%
REM rozsah 1-5, bez parametru je default 3


