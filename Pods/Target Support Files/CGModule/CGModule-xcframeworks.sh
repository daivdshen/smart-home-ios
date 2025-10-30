#!/bin/sh
set -e
set -u
set -o pipefail

function on_error {
  echo "$(realpath -mq "${0}"):$1: error: Unexpected failure"
}
trap 'on_error $LINENO' ERR


# This protects against multiple targets copying the same framework dependency at the same time. The solution
# was originally proposed here: https://lists.samba.org/archive/rsync/2008-February/020158.html
RSYNC_PROTECT_TMP_FILES=(--filter "P .*.??????")


variant_for_slice()
{
  case "$1" in
  "CGModule.xcframework/ios-arm64")
    echo ""
    ;;
  "CGModule.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "CMGMultimedia.xcframework/ios-arm64")
    echo ""
    ;;
  "CMGMultimedia.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "ComelitVipKit.xcframework/ios-arm64")
    echo ""
    ;;
  "ComelitVipKit.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "ComelitCoreKit.xcframework/ios-arm64")
    echo ""
    ;;
  "ComelitCoreKit.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "CMGTolo.xcframework/ios-arm64")
    echo ""
    ;;
  "CMGTolo.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "VipComelit.xcframework/ios-arm64")
    echo ""
    ;;
  "VipComelit.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "libp2p.xcframework/ios-arm64")
    echo ""
    ;;
  "libp2p.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "Nebula.xcframework/ios-arm64")
    echo ""
    ;;
  "Nebula.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "cspstack.xcframework/ios-arm64")
    echo ""
    ;;
  "cspstack.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "cmp.xcframework/ios-arm64")
    echo ""
    ;;
  "cmp.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "cmpot.xcframework/ios-arm64")
    echo ""
    ;;
  "cmpot.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "comelitutils.xcframework/ios-arm64")
    echo ""
    ;;
  "comelitutils.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "Mosquitto.xcframework/ios-arm64")
    echo ""
    ;;
  "Mosquitto.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "libavdevice.xcframework/ios-arm64")
    echo ""
    ;;
  "libavdevice.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "libavformat.xcframework/ios-arm64")
    echo ""
    ;;
  "libavformat.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "libavutil.xcframework/ios-arm64")
    echo ""
    ;;
  "libavutil.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "libswscale.xcframework/ios-arm64")
    echo ""
    ;;
  "libswscale.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "libavcodec.xcframework/ios-arm64")
    echo ""
    ;;
  "libavcodec.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "libavfilter.xcframework/ios-arm64")
    echo ""
    ;;
  "libavfilter.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "libavresample.xcframework/ios-arm64")
    echo ""
    ;;
  "libavresample.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "libswresample.xcframework/ios-arm64")
    echo ""
    ;;
  "libswresample.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "openssl.xcframework/ios-arm64")
    echo ""
    ;;
  "openssl.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "pjproject.xcframework/ios-arm64")
    echo ""
    ;;
  "pjproject.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  "curl.xcframework/ios-arm64")
    echo ""
    ;;
  "curl.xcframework/ios-arm64_x86_64-simulator")
    echo "simulator"
    ;;
  esac
}

archs_for_slice()
{
  case "$1" in
  "CGModule.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "CGModule.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "CMGMultimedia.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "CMGMultimedia.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "ComelitVipKit.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "ComelitVipKit.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "ComelitCoreKit.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "ComelitCoreKit.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "CMGTolo.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "CMGTolo.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "VipComelit.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "VipComelit.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "libp2p.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "libp2p.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "Nebula.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "Nebula.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "cspstack.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "cspstack.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "cmp.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "cmp.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "cmpot.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "cmpot.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "comelitutils.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "comelitutils.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "Mosquitto.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "Mosquitto.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "libavdevice.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "libavdevice.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "libavformat.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "libavformat.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "libavutil.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "libavutil.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "libswscale.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "libswscale.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "libavcodec.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "libavcodec.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "libavfilter.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "libavfilter.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "libavresample.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "libavresample.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "libswresample.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "libswresample.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "openssl.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "openssl.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "pjproject.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "pjproject.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  "curl.xcframework/ios-arm64")
    echo "arm64"
    ;;
  "curl.xcframework/ios-arm64_x86_64-simulator")
    echo "arm64 x86_64"
    ;;
  esac
}

copy_dir()
{
  local source="$1"
  local destination="$2"

  # Use filter instead of exclude so missing patterns don't throw errors.
  echo "rsync --delete -av "${RSYNC_PROTECT_TMP_FILES[@]}" --links --filter \"- CVS/\" --filter \"- .svn/\" --filter \"- .git/\" --filter \"- .hg/\" \"${source}*\" \"${destination}\""
  rsync --delete -av "${RSYNC_PROTECT_TMP_FILES[@]}" --links --filter "- CVS/" --filter "- .svn/" --filter "- .git/" --filter "- .hg/" "${source}"/* "${destination}"
}

SELECT_SLICE_RETVAL=""

select_slice() {
  local xcframework_name="$1"
  xcframework_name="${xcframework_name##*/}"
  local paths=("${@:2}")
  # Locate the correct slice of the .xcframework for the current architectures
  local target_path=""

  # Split archs on space so we can find a slice that has all the needed archs
  local target_archs=$(echo $ARCHS | tr " " "\n")

  local target_variant=""
  if [[ "$PLATFORM_NAME" == *"simulator" ]]; then
    target_variant="simulator"
  fi
  if [[ ! -z ${EFFECTIVE_PLATFORM_NAME+x} && "$EFFECTIVE_PLATFORM_NAME" == *"maccatalyst" ]]; then
    target_variant="maccatalyst"
  fi
  for i in ${!paths[@]}; do
    local matched_all_archs="1"
    local slice_archs="$(archs_for_slice "${xcframework_name}/${paths[$i]}")"
    local slice_variant="$(variant_for_slice "${xcframework_name}/${paths[$i]}")"
    for target_arch in $target_archs; do
      if ! [[ "${slice_variant}" == "$target_variant" ]]; then
        matched_all_archs="0"
        break
      fi

      if ! echo "${slice_archs}" | tr " " "\n" | grep -F -q -x "$target_arch"; then
        matched_all_archs="0"
        break
      fi
    done

    if [[ "$matched_all_archs" == "1" ]]; then
      # Found a matching slice
      echo "Selected xcframework slice ${paths[$i]}"
      SELECT_SLICE_RETVAL=${paths[$i]}
      break
    fi
  done
}

install_xcframework() {
  local basepath="$1"
  local name="$2"
  local package_type="$3"
  local paths=("${@:4}")

  # Locate the correct slice of the .xcframework for the current architectures
  select_slice "${basepath}" "${paths[@]}"
  local target_path="$SELECT_SLICE_RETVAL"
  if [[ -z "$target_path" ]]; then
    echo "warning: [CP] $(basename ${basepath}): Unable to find matching slice in '${paths[@]}' for the current build architectures ($ARCHS) and platform (${EFFECTIVE_PLATFORM_NAME-${PLATFORM_NAME}})."
    return
  fi
  local source="$basepath/$target_path"

  local destination="${PODS_XCFRAMEWORKS_BUILD_DIR}/${name}"

  if [ ! -d "$destination" ]; then
    mkdir -p "$destination"
  fi

  copy_dir "$source/" "$destination"
  echo "Copied $source to $destination"
}

install_xcframework "${PODS_ROOT}/CGModule/CGModule.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/CMGMultimedia.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/ComelitVipKit.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/ComelitCoreKit.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/CMGTolo.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/VipComelit.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/libp2p.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/Nebula.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/cspstack.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/cmp.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/cmpot.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/comelitutils.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/Mosquitto.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/libavdevice.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/libavformat.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/libavutil.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/libswscale.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/libavcodec.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/libavfilter.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/libavresample.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/libswresample.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/openssl.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/pjproject.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"
install_xcframework "${PODS_ROOT}/CGModule/curl.xcframework" "CGModule" "framework" "ios-arm64" "ios-arm64_x86_64-simulator"

