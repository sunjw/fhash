#!/bin/bash

if [ -z "$1" ]
  then
    echo "Usage: package_dmg.sh [/some/target/dir]"
    exit
fi

cur_dir=$PWD
vol_name="fHash"
src_folder=$1
temp_dmg_path="fHash.dmg"

echo "Create DMG from [$src_folder]..."

# Clean
rm -f "$temp_dmg_path"

cd "$src_folder"

# Clean
rm -f .DS_Store
# Make /Applications link
ln -s /Applications Applications

cd "$cur_dir"

# Create DMG
hdiutil create -format UDBZ -fs HFS+ -volname "$vol_name" -srcfolder "$src_folder" "$temp_dmg_path"

# Move to target directory
echo "Move DMG to [$src_folder]"
mv "$temp_dmg_path" "$src_folder"

cd "$src_folder"

# Clean
rm -f Applications

cd "$cur_dir"

echo "Done"

