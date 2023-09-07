#!/bin/bash

# Function to check dependencies for each architecture
check_dependencies() {
    declare -a packages=("gcc" "g++" "make")

    if [ "$1" == "x86_64" ]; then
        packages+=("gcc-multilib" "g++-multilib")
    elif [ "$1" == "ARM64" ]; then
        packages+=("gcc-aarch64-linux-gnu" "g++-aarch64-linux-gnu")
    elif [ "$1" == "ARM" ]; then
        packages+=("gcc-arm-linux-gnueabi" "g++-arm-linux-gnueabi")
    elif [ "$1" == "ARMHF" ]; then
            packages+=("gcc-arm-linux-gnueabihf" "g++-arm-linux-gnueabihf")
    elif [ "$1" == "x86" ]; then
        packages+=("gcc-multilib" "g++-multilib")
    fi

    for pkg in "${packages[@]}"; do
        dpkg -l | grep -qw "$pkg" || missing_packages+="$pkg "
    done

    if [ -n "$missing_packages" ]; then
        echo "Missing packages: $missing_packages"
        echo "Installing missing packages..."
        sudo apt-get install -y $missing_packages
    fi
}

# Configuration Menu
HEIGHT=15
WIDTH=40
CHOICE_HEIGHT=4
BACKTITLE="Function Hooks Compilation Options"
TITLE="Configurations"
MENU="Choose the architecture:"

OPTIONS=(1 "x86_64"
         2 "x86"
         3 "ARM64"
         4 "ARM"
         5 "ARMHF")

CHOICE=$(dialog --clear \
                --backtitle "$BACKTITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 >/dev/tty)

clear

# Generate config.mk based on user input
case $CHOICE in
    1)
        check_dependencies "x86_64"
        echo "ARCH=x86_64" > config.mk
        echo "CFLAGS=-Wall -fPIC" >> config.mk
        ;;
    2)
        check_dependencies "x86"
        echo "ARCH=x86" > config.mk
        echo "CFLAGS=-Wall -fPIC -m32" >> config.mk
        ;;
    3)
        check_dependencies "ARM64"
        echo "ARCH=ARM64" > config.mk
        echo "CROSS_COMPILE=aarch64-linux-gnu-" >> config.mk
        echo "CFLAGS=-Wall -fPIC" >> config.mk
        ;;
    4)
        check_dependencies "ARM"
        echo "ARCH=ARM" > config.mk
        echo "CROSS_COMPILE=arm-linux-gnueabi-" >> config.mk
        echo "CFLAGS=-Wall -fPIC" >> config.mk
        ;;
    5)
        check_dependencies "ARMHF"
        echo "ARCH=ARM" > config.mk
        echo "CROSS_COMPILE=arm-linux-gnueabihf-" >> config.mk
        echo "CFLAGS=-Wall -fPIC" >> config.mk
        ;;
esac
