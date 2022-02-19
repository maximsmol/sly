#!/bin/bash
export PATH="/usr/local/opt/llvm/bin:$PATH"
runhaskell -- shake.hs $@
