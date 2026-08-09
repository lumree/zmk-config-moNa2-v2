/*
 * ZMK v0.4 互換シム
 *
 * ZMK v0.4 (cormoran/zmk main+dya) で endpoints API が改名され、
 *   zmk_endpoints_send_mouse_report()  (v0.3)
 *     -> zmk_endpoint_send_mouse_report()  (v0.4)
 * となった。
 *
 * shakushakupanda/zmk-module-mouse-gesture-rpc の
 * src/input_processors/input_processor_inertial_scroll.c は旧名を呼んでいるため、
 * そのままだとリンク時に undefined reference になる。
 *
 * 本来はモジュール側 (自分の fork) を直すのが正しい。
 * 直し方は docs/mouse-gesture-rpc-zmk-v0.4.patch を参照。
 * モジュール側を修正したら、このファイルと CMakeLists.txt の該当行、
 * zephyr/module.yml の `cmake: .` は削除してよい。
 *
 * NOTE: <zmk/endpoints.h> は include しない。
 *       このシムは Zephyr モジュールとして独立したライブラリにビルドされ、
 *       ZMK app の include ディレクトリが通っていないため。
 *       必要なのは 1 関数のプロトタイプだけなので自前で宣言する。
 */

int zmk_endpoint_send_mouse_report(void);
int zmk_endpoints_send_mouse_report(void);

int zmk_endpoints_send_mouse_report(void) {
    return zmk_endpoint_send_mouse_report();
}
