#!/bin/bash

make
./woody_woodpacker test && ./to_change_woody && python -c "print 'OMG' * 4000"

