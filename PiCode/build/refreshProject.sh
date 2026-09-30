bash
#!/bin/bash
# Clean the build directory and regenerate the Eclipse project.
# Keeps only readme.txt, .gitignore, and the shell scripts; everything else is removed,
# including hidden files (.project, .cproject, .settings) and subdirectories.

set -e
cd "$(dirname "$0")"          # always operate on the build dir, wherever it's run from

shopt -s extglob dotglob nullglob
rm -rf -- !(*.sh|readme.txt|.gitignore)

bash createProject.sh
