#!/bin/sh

find . -type f -exec file {} \; | grep -i elf | grep -i executable | cut -f 1 -d ':'
#!/bin/sh

find . -type f -exec file {} \; | grep -i elf | grep -i executable | cut -f 1 -d ':'
