#!/bin/bash

if [ -z "$1" ]
  then
    echo "Usage: package_dmg.sh </some/archive/Applications_contains_.app>"
    exit
fi

cur_dir=$PWD
app_title="fHash"
app_name="$app_title.app"
vol_name=$app_title
src_folder=$1
temp_dmg_path="$app_title-VERSION-macOS.dmg"

echo "Create DMG from [$src_folder]..."

cd "$src_folder"

# Clean
rm -f .DS_Store
# Make /Applications link
ln -s /Applications Applications
# Get version
plist_version=$(/usr/libexec/PlistBuddy -c "Print :CFBundleShortVersionString" "$app_name/Contents/Info.plist")
temp_dmg_path=${temp_dmg_path/VERSION/$plist_version}

cd "$cur_dir"

# Create DMG
hdiutil create -format UDZO -fs HFS+ -volname "$vol_name" -srcfolder "$src_folder" "$temp_dmg_path"

# Move to target directory
echo "Move DMG to [$src_folder]"
mv "$temp_dmg_path" "$src_folder"

cd "$src_folder"

# Clean
rm -f Applications

# Tar
tar -cJvf "$app_name.tar.xz" "$app_name"

cd "$cur_dir"

echo "Done"

