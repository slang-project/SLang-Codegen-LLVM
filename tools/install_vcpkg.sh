#!/bin/sh -e

if [ "$#" -ge 1 ]; then
    vcpkgRoot="$1"
else
    toolsDir=$(X= cd -- "$(dirname -- "$0")" && pwd -P)
    projectRoot=$(X= cd -- "$toolsDir/.." && pwd -P)
    vcpkgRoot="$(X= cd -- \"$projectRoot/..\" && pwd -P)/vcpkg"
fi

checkIfRequiredToolExists() {
    __tool=$1
    if ! command -v "$__tool" >/dev/null 2>&1 ; then
        echo "Could not find '$__tool'. Please install it (and other dependencies) with:"
        echo "sudo apt-get install git"
        exit 1
    fi
}

ensurePathDoesNotExist() {
    __path=$1
    if [ -e "$__path" ]; then
        echo "Path '$__path' already exists. Exiting..."
        exit 1
    fi
}

setGlobalEnvironmentVariable() {
    varName=$1; varValue=$2

    UNAME="$(uname)"

    if [ "$UNAME" = "Linux" ]; then
        echo "$varName=\"$varValue\"" >> $HOME/.profile
    elif [ "$UNAME" = "Darwin" ]; then
        launchctl setenv $varName "$varValue"
    else
        echo "Unknown uname: $UNAME"
        echo "Global environment variable '$varName' was not set"
        return 1
    fi
}

ensurePathDoesNotExist "$vcpkgRoot"

checkIfRequiredToolExists "git"
git clone https://github.com/microsoft/vcpkg "$vcpkgRoot"
vcpkgRoot=$(X= cd -- "$vcpkgRoot" && pwd -P)
"$vcpkgRoot/bootstrap-vcpkg.sh"

echo "Installed 'vcpkg' successfully at path '$vcpkgRoot'"

setGlobalEnvironmentVariable "VCPKG_ROOT" "$vcpkgRoot"

echo "You can access 'vcpkg' path via 'VCPKG_ROOT' environment variable"
