#!/bin/bash
#
#  Build Script for CarboniteKERNEL for the OnePlus 5!
#  Based off RenderBroken's build script which is...
#  ...based off AK's build script ~~ Thanks!
#
#  git log --oneline --decorate
#

# Resources
THREAD="-j$(grep -c ^processor /proc/cpuinfo)"
KERNEL="Image.gz-dtb"
DEFCONFIG="flash_defconfig"

VER="Adins-Kernel-OOS-R1"
VARIANT="OP5-N"

# Paths
ANDROID_DIR="${HOME}"
KERNEL_DIR="$ANDROID_DIR/kernels/op5"
REPACK_DIR="$ANDROID_DIR/kernels/anykernel2"
MODULES_DIR="$REPACK_DIR/modules"
ZIP_MOVE="$ANDROID_DIR/kernels/out/op5"
ZIMAGE_DIR="$KERNEL_DIR/arch/arm64/boot"

# Vars
export ARCH=arm64
export SUBARCH=arm64
export LOCALVERSION=-`echo $VER`
export CROSS_COMPILE="$ANDROID_DIR/carbon/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-androidkernel-"

function clean_all {
  echo
  make clean O=out && make mrproper O=out
  if [ -f "$ZIMAGE_DIR/$KERNEL" ]
  then
    rm $ZIMAGE_DIR/$KERNEL
  fi
}

function make_kernel {
  echo
  make $DEFCONFIG O=out
  make $THREADO=out
}

function make_modules {
  rm `echo $MODULES_DIR"/*"`
  find $KERNEL_DIR -name '*.ko' -exec cp {} $MODULES_DIR \;
  ${CROSS_COMPILE}strip --strip-unneeded $MODULES_DIR/*
}

function make_zip {
  cd $REPACK_DIR
  zip -r9 "$VER"-"$VARIANT".zip *
  mv "$VER"-"$VARIANT".zip $ZIP_MOVE
  cd $KERNEL_DIR
}

if [ -f "$REPACK_DIR/zImage" ]
then
  rm -rf $REPACK_DIR/zImage
fi

clear
echo "CarboniteKERNEL build script for $ROM:"
echo

if [ -f "$KERNEL_DIR/.config" ]
then
  while read -p "Do you want to clean stuffs (y/n)? " cchoice
  do
    case "$cchoice" in
      y|Y )
        clean_all
        echo
        echo "All Cleaned now."
        break
        ;;
      n|N )
        break
        ;;
      * )
        echo
        echo "Invalid try again!"
        echo
        ;;
    esac
  done
  echo
fi

while read -p "Do you want to build kernel (y/n)? " dchoice
do
  case "$dchoice" in
    y|Y)
      DATE_START=$(date +"%s")
      make_kernel
      if [ -f "$ZIMAGE_DIR/$KERNEL" ]
      then
        cp $ZIMAGE_DIR/$KERNEL $REPACK_DIR/zImage
        make_modules
        make_zip
        echo
        echo "-----------------------------------------"
        echo "-Kernel image build successfull for $ROM-"
        echo "-----------------------------------------"
        echo
      else
        echo "-------------------------------------------"
        echo "-Kernel image build unsuccessfull for $ROM-"
        echo "-------------------------------------------"
      fi
      DATE_END=$(date +"%s")
      DIFF=$(($DATE_END - $DATE_START))
      echo "Time: $(($DIFF / 60)) minute(s) and $(($DIFF % 60)) seconds."
      break
      ;;
    n|N )
      break
      ;;
    * )
      echo
      echo "Invalid try again!"
      echo
      ;;
  esac
done

echo
if [ -f "$ZIP_MOVE/$VER-$VARIANT.zip" ]
then
  echo "$ZIP_MOVE/$VER-$VARIANT.zip"
else
  echo "No zip was found, build incomplete."
fi
echo
