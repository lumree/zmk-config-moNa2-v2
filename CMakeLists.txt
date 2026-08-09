# このリポジトリを Zephyr モジュールとしてビルドするための定義。
# 主目的は boards/shields/mona2 の提供 (zephyr/module.yml の board_root) だが、
# ZMK v0.4 移行に伴う互換シムもここでビルドする。
if(CONFIG_ZMK_MOUSE_GESTURE_RPC)
  zephyr_library_named(mona2_compat)
  zephyr_library_sources(compat/zmk_v0_4_endpoints_compat.c)
endif()
