# moNa2 v2 (DYA Studio 対応版)

このリポジトリは [moNa2 v2](https://github.com/sayu-hub/zmk-config-moNa2) 用 ZMK Config を、[cormoran](https://github.com/cormoran) さんが公開している **[DYA Studio](https://studio.dya.cormoran.works/)** に対応させたものです。

DYA Studio は ZMK Studio をベースに、

- キーマップ／レイヤーの GUI 編集
- **マクロの作成・編集・削除（ファームウェア再ビルド不要）**
- **コンボの追加・編集・削除（ファームウェア再ビルド不要）**
- トラックボール (PMW3610) のスクロール量・軸反転・自動マウスレイヤーなど **Runtime Input Processor 経由での動的設定**
- BLE プロファイル管理 / Settings RPC

を Web から行えるようにした拡張版です。

## DYA Studio の使い方

1. 本リポジトリをビルドし、生成された `mona2_r-...uf2`（中央側＝右手）と `mona2_l-...uf2`（周辺側＝左手）をそれぞれの XIAO BLE に書き込みます。
2. 中央側（右手）を **USB ケーブル** で PC に接続します。
3. Chrome / Edge などの WebUSB 対応ブラウザで **[https://studio.dya.cormoran.works/](https://studio.dya.cormoran.works/)** を開きます。
4. キーボード側で `BLE` レイヤー(レイヤー10/11)を有効にし、右下に配置した **`&studio_unlock`** キーを押してアンロック。
5. DYA Studio 側で「Connect」を押し、USB デバイスとして mona2 を選択。
6. キーマップ／マクロ／コンボ／トラックボール設定を編集できます。

### マクロを DYA Studio から使う

マクロのランタイム編集は [zmk-feature-runtime-macro](https://github.com/cormoran/zmk-feature-runtime-macro) が担当します。

1. DYA Studio のマクロ画面で新しいマクロを作成し、名前と内容を設定して保存します。
2. マクロ一覧に表示される **スロット番号 (0〜7)** を確認します。
3. DYA Studio のキーマップエディタで、任意のキーに `&rmacro <スロット番号>` を割り当てます。

キーマップ側にはすでに `#include <behaviors/runtime_macro.dtsi>` を入れてあるので、`&rmacro` は最初から選択できます。

既存の `&BT0` / `&screenshot` / `&henkan` などは従来どおりの **静的マクロ** のままで、DYA Studio からは編集できません（キーマップに直接名前で書かれているため、そのまま残しています）。編集したくなったら DYA Studio 側で同じ内容のランタイムマクロを作り直してキーを差し替えてください。

### コンボを DYA Studio から使う

コンボは [zmk-feature-runtime-combo](https://github.com/cormoran/zmk-feature-runtime-combo) に移行済みです。

`config/mona2.keymap` の `runtime_combo_defaults` ノードが **コンパイル時デフォルト**（フラッシュ直後から有効・設定消去でも復活）で、現状は次の 2 つが入っています。

| スロット | 内容 | キー位置 |
| --- | --- | --- |
| 0 | `&lt 4 ESC` | 38, 39 |
| 1 | `&kp TAB` | 11, 12 |

スロットは全部で 16 個 (`CONFIG_ZMK_RUNTIME_COMBO_MAX_COMBOS`) あるので、残り 14 個は DYA Studio から自由に追加できます。既存スロットを Studio 上で書き換えた場合は、Web UI の **Reset to Default** でここの値へ戻せます。

### このリポジトリで対応済みの内容

| 対応項目 | ファイル / 箇所 |
| --- | --- |
| ZMK 本体を cormoran さんの fork (`main+dya`, ZMK v0.4 / Zephyr 4.1 系) に切替 | `config/west.yml` |
| DYA Studio 基盤モジュール (custom-settings / ble-management / settings-rpc / runtime-input-processor / runtime-sensor-rotate) | `config/west.yml` |
| **マクロ・コンボのランタイム編集モジュール (runtime-macro / runtime-combo) を追加** | `config/west.yml` |
| Studio 系 CONFIG (`CONFIG_ZMK_STUDIO`, `CONFIG_ZMK_CUSTOM_SETTINGS`, `CONFIG_ZMK_BLE_MANAGEMENT`, `CONFIG_ZMK_SETTINGS_RPC`, `CONFIG_ZMK_RUNTIME_INPUT_PROCESSOR`, `CONFIG_ZMK_RUNTIME_MACRO`, `CONFIG_ZMK_RUNTIME_COMBO` ほか) | `config/mona2_r.conf` |
| 静的コンボ (`zmk,combos`) を `runtime_combo_defaults` へ移行 | `config/mona2.keymap` |
| `&rmacro` (ランタイムマクロ behavior) を利用可能に | `config/mona2.keymap` |
| トラックボール処理を Runtime Input Processor に置換 | `boards/shields/mona2/mona2.dtsi`, `mona2_r.overlay` |
| `&studio_unlock` を `ble_win` / `ble_mac` レイヤー右上に配置 | `config/mona2.keymap` |
| 全レイヤーに `display-name` を設定 | `config/mona2.keymap` |
| `studio-rpc-usb-uart` snippet (中央側) / ボード名を `xiao_ble/nrf52840/zmk` へ | `build.yaml` |

> **Note:**
> - DYA Studio で行った変更は **中央側 (右手) の Flash** に保存されます。初期化したい場合は `settings_reset` ファームウェアを書き込んでください。
> - **ZMK v0.4 系への移行に伴い、以前の設定（保存済みキーマップ・トラックボール設定）は引き継がれません。** 書き込み後に DYA Studio から設定し直してください。

## ZMK v0.4 系への移行で変わった点

`v0.3-branch+dya` (Zephyr 3.5) から `main+dya` (Zephyr 4.1) に上げたことで、以下の破壊的変更に追従しています。

| 変更 | 対応 |
| --- | --- |
| ボード名 `seeeduino_xiao_ble` → `xiao_ble`、ZMK 用バリアント `/nrf52840/zmk` が必須 | `build.yaml` |
| Zephyr 4.1 が純正 `pixart,pmw3610` ドライバを同梱したため、badjeff さんのモジュールは compatible が `pixart,pmw3610-alt`、CONFIG が `CONFIG_PMW3610_ALT_*` に改名 | `mona2_r.overlay`, `Kconfig.defconfig`, `mona2_r.conf`, `west.yml` (`zmk-0.3` → `zmk-0.4`) |
| `zmk-rgbled-widget` の `v0.3` ブランチは Zephyr 3.5 向け | `west.yml` (`v0.3` → `main`) |
| endpoints API が `zmk_endpoints_send_mouse_report()` → `zmk_endpoint_send_mouse_report()` に改名 | `compat/zmk_v0_4_endpoints_compat.c` の互換シム（詳細は下記） |
| 再利用 GitHub Actions ワークフローが `@v0.3.0` のままだと Zephyr 4.1 でビルドできない | `.github/workflows/build.yml` (`@main`) |

### mouse-gesture-rpc の互換シムについて

`shakushakupanda/zmk-module-mouse-gesture-rpc` (ブランチ `expose-mg-set-to-studio`) が
ZMK v0.4 で消えた `zmk_endpoints_send_mouse_report()` を呼んでいるため、
そのままではリンクエラーになります。

暫定対応として本リポジトリに互換シム (`compat/zmk_v0_4_endpoints_compat.c`) を置いています。
**本来はモジュール側を直すのが正しい**ので、`docs/mouse-gesture-rpc-zmk-v0.4.patch` を
自分の fork に適用してください。適用後はシム関連（`CMakeLists.txt` の該当ブロック、
`compat/`、`zephyr/module.yml` の `cmake: .`）を削除できます。

### PAW3222 版ビルドについて（既存不具合の修正）

Zephyr は devicetree を Kconfig より **先に** 処理するため、`mona2_r.overlay` 内の
`#ifdef CONFIG_TRACKBALL_PAW3222` はクリーンビルドでは常に偽になります。
その結果、これまで `mona2_r-paw3222` という名前で生成されていた成果物は
**実際には PMW3610 版**になっていました。

`config/paw3222.overlay` を追加し、`build.yaml` から `-DEXTRA_DTC_OVERLAY_FILE` で
渡すことで、正しく `pixart,paw3222` が選ばれるよう修正しています。

### ビルド検証状況

このブランチは実機書き込み前に、ローカル (Zephyr SDK 0.17.0 / arm-zephyr-eabi) で
以下 4 構成のビルドが通ることを確認しています。

| 構成 | 結果 | FLASH | RAM |
| --- | --- | --- | --- |
| `mona2_r rgbled_adapter` (PMW3610) | OK | 45.70% | 55.16% |
| `mona2_r rgbled_adapter` (PAW3222) | OK | 45.62% | 55.16% |
| `mona2_l rgbled_adapter` | OK | 24.71% | 18.23% |
| `settings_reset` | OK | 7.31% | 6.66% |

**実機での動作確認（特にトラックボール、マウスジェスチャー、エンコーダ、BLE ペアリング）は未実施です。**

---

## トラックボール (COROPIT) を使用する場合

COROPITを使用する方は以下のようにコードを編集してください。

mona2_r.overlay

修正前
```
  trackball_central: trackball_central@0 {
        status = "okay";
        compatible = "pixart,pmw3610";  //トラボセンサ用のドライバとバインド
        reg = <0>;
        spi-max-frequency = <2000000>;
        irq-gpios = <&gpio0 2 (GPIO_ACTIVE_LOW | GPIO_PULL_UP)>; //P0.02を指定(MOTION)
        cpi = <600>;
        //swap-xy;
        //invert-x; //COROPIT版ではコメントアウトを外す
        //invert-y; //COROPIT版ではコメントアウトを外す
        evt-type = <INPUT_EV_REL>;
        x-input-code = <INPUT_REL_X>;
        y-input-code = <INPUT_REL_Y>;
    };
};

```
**修正後**
```
  trackball_central: trackball_central@0 {
        status = "okay";
        compatible = "pixart,pmw3610";  //トラボセンサ用のドライバとバインド
        reg = <0>;
        spi-max-frequency = <2000000>;
        irq-gpios = <&gpio0 2 (GPIO_ACTIVE_LOW | GPIO_PULL_UP)>; //P0.02を指定(MOTION)
        cpi = <600>;
        //swap-xy;
        invert-x; //COROPIT版ではコメントアウトを外す
        invert-y; //COROPIT版ではコメントアウトを外す
        evt-type = <INPUT_EV_REL>;
        x-input-code = <INPUT_REL_X>;
        y-input-code = <INPUT_REL_Y>;
    };
};

```
