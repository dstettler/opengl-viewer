archdeps=(
    "/usr/lib/libOpenGL.so"
    "/usr/lib/libGLU.so"
    "/usr/lib/libGLX.so"
    "/usr/lib/libICE.so"
    "/usr/lib/libSM.so"
    "/usr/lib/libXext.so"
    "/usr/lib/libX11.so"
    "/usr/lib/libm.so"
    "/usr/lib/librt.a"
    "/usr/lib/libGLEW.so"
    "/usr/lib/libXcursor.so"
    "/usr/lib/libXrandr.so"
)

ubuntudeps=(
    "/usr/lib/x86_64-linux-gnu/libOpenGL.so"
    "/usr/lib/x86_64-linux-gnu/libGLU.so"
    "/usr/lib/x86_64-linux-gnu/libGLX.so"
    "/usr/lib/x86_64-linux-gnu/libICE.so"
    "/usr/lib/x86_64-linux-gnu/libSM.so"
    "/usr/lib/x86_64-linux-gnu/libXext.so"
    "/usr/lib/x86_64-linux-gnu/libX11.so"
    "/usr/lib/x86_64-linux-gnu/libm.so"
    "/usr/lib/x86_64-linux-gnu/librt.a"
    "/usr/lib/x86_64-linux-gnu/libGLEW.so"
)

# From https://unix.stackexchange.com/questions/6345/how-can-i-get-distribution-name-and-version-number-in-a-simple-shell-script
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$NAME
fi
echo $OS

if [ "$OS" = "Ubuntu" ]; then
    for f in ${ubuntudeps[@]}; do
        if [ -f $f ]; then
            echo "Found: ${f}"
        else
            echo "Missing: ${f}"
        fi
    done
elif [ "$OS" = "Arch Linux" ]; then
        for f in ${archdeps[@]}; do
        if [ -f $f ]; then
            echo "Found: ${f}"
        else
            echo "Missing: ${f}"
        fi
    done
fi