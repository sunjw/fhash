#!/bin/bash

if [ -z "$1" ]
  then
    echo "Don't forget \"npm install --global create-dmg\""
    echo "Usage: package_dmg.sh </some/archive/Applications_contains_.app>"
    exit
fi

cur_dir=$PWD
application_folder=$1

echo "Create DMG from [$application_folder]..."

cd "$application_folder"

app_name=$(find . -maxdepth 1 -type d -name "*.app" | sort | head -n 1)

if [ -z "$app_name" ]; then
  echo "No .app found in [$application_folder]"
  cd "$cur_dir"
  exit 1
fi

app_name=${app_name#./}
app_title=${app_name%.app}
vol_name=$app_title
temp_dmg_name="$app_title-VERSION-macOS.dmg"

# Get version
plist_version=$(/usr/libexec/PlistBuddy -c "Print :CFBundleShortVersionString" "$app_name/Contents/Info.plist")
temp_dmg_name=${temp_dmg_name/VERSION/$plist_version}

echo "Found app [$app_name], version [$plist_version]"

# Clean
rm -f .DS_Store

# # Make /Applications link
# ln -s /Applications Applications

# cd "$cur_dir"

# # Create DMG
# hdiutil create -format UDZO -fs HFS+ -volname "$vol_name" -srcfolder "$application_folder" "$temp_dmg_name"

# # Move to target directory
# echo "Move DMG to [$application_folder]"
# mv "$temp_dmg_name" "$application_folder"

# cd "$application_folder"

# # Clean
# rm -f Applications

# Call create-dmg
# Don't forget "npm install --global create-dmg"
create-dmg --overwrite --no-code-sign "$app_name"

# Rename dmg
dmg_file=$(ls *.dmg)
# echo $dmg_file
mv "$dmg_file" "$temp_dmg_name"

# Tar
tar -cJvf "$app_name-$plist_version.tar.xz" "$app_name"

cd "$cur_dir"

echo "Done"

