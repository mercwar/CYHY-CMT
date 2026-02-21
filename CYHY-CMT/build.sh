#!/usr/bin/env bash
set -e

gcc -mwindows \
    cyhy.c \
    mercg.c \
    template_loader.c \
    avis-log.c \
    -o cyhy.exe
