#!/bin/sh

export MAGICK='magick'
if ! command -v $MAGICK 2>&1 >/dev/null
then
    export MAGICK='convert'
    if ! command -v $MAGICK 2>&1 >/dev/null
    then
        export MAGICK="docker run --entrypoint=magick -v $PWD:/imgs dpokidov/imagemagick"
        if ! command -v $MAGICK 2>&1 >/dev/null
        then
            echo "Error: ImageMagick is not installed and cannot be run in a container."
            exit 1
        fi
    fi
fi

if [[ "$OSTYPE" == "darwin"* ]]; then
    SED_INPLACE=(-i '')
else
    SED_INPLACE=(-i)
fi

for font in assets/fonts/*.ttf; do
    FOLDER=$(echo "$font" | sed 's/\.ttf$//')
    if [ -d "$FOLDER" ]; then continue; fi
    mkdir -p $FOLDER
	echo -e "\033[1;32mCompiling font \033[1;0m\"$font\"\033[1;32m into \033[1;0m\"$FOLDER\"\033[1;32m.\033[0m"
    for i in $(seq 0 255); do
        char=$(printf "\\$(printf '%03o' "$i")")
        $MAGICK -size 16x16 xc:none -font "$font" \
            -pointsize 12 -gravity center -fill "#FFFFFFF1" \
            -annotate 0 "$char" "$FOLDER/$i.xpm"
    done
    find "$FOLDER" -name '*.xpm' -exec sed "${SED_INPLACE[@]}" 's/ c white/ c #FEFFFFFF/g' {} +
    find "$FOLDER" -name '*.xpm' -exec sed "${SED_INPLACE[@]}" 's/ c None/ c #00000000/g' {} +
done
