    #include "ble_manager.h"
    #include "wifi_manager.h"
    #include <stdio.h>
    #include <string.h>

    #include "nimble/nimble_port.h"
    #include "nimble/nimble_port_freertos.h"

    #include "host/ble_hs.h"
    #include "host/ble_gap.h"
    #include "host/ble_gatt.h"


    static uint8_t own_addr_type;

    static void start_advertising(void);
    static int write_callback(uint16_t conn_handle,
                            uint16_t attr_handle,
                            struct ble_gatt_access_ctxt *ctxt,
                            void *arg)
    {
        char buffer[256] = {0};

        ble_hs_mbuf_to_flat(
            ctxt->om,
            buffer,
            sizeof(buffer) - 1,
            NULL);

        printf("Received: %s\n", buffer);

        char ssid[32];
        char password[64];

        if (sscanf(buffer,
                "{\"SSID\":\"%31[^\"]\",\"Password\":\"%63[^\"]\"}",
                ssid,
                password) == 2)
        {
            printf("SSID = %s\n", ssid);
            printf("PASSWORD = %s\n", password);
            save_wifi_credentials(ssid, password);
            wifi_manager_connect(ssid, password);
        }
        else
        {
            printf("Invalid Input Format\n");
        }

        return 0;
    }

    static const struct ble_gatt_svc_def gatt_svcs[] =
    {
        {
            .type = BLE_GATT_SVC_TYPE_PRIMARY,

            .uuid = BLE_UUID128_DECLARE(
                0x78,0x56,0x34,0x12,
                0x34,0x12,
                0x78,0x56,
                0x34,0x12,
                0xef,0xcd,0xab,0x89,0x67,0x45),

            .characteristics =
            (struct ble_gatt_chr_def[])
            {
                {
                    .uuid = BLE_UUID128_DECLARE(
                        0x11,0x11,0x11,0x11,
                        0x11,0x11,
                        0x11,0x11,
                        0x11,0x11,
                        0x11,0x11,0x11,0x11,0x11,0x11),

                    .access_cb = write_callback,

                    .flags =
                        BLE_GATT_CHR_F_WRITE |
                        BLE_GATT_CHR_F_WRITE_NO_RSP,
                },

                {0}
            }
        },

        {0}
    };
    static int gap_event(struct ble_gap_event *event, void *arg)
    {
        switch (event->type)
        {
            case BLE_GAP_EVENT_CONNECT:
                printf("CONNECTED\n");
                break;

            case BLE_GAP_EVENT_DISCONNECT:
                printf("DISCONNECTED\n");

                start_advertising();   // restart advertising
                break;
        }

        return 0;
    }

    static void start_advertising(void)
    {
        struct ble_hs_adv_fields fields = {0};

        fields.flags =
            BLE_HS_ADV_F_DISC_GEN |
            BLE_HS_ADV_F_BREDR_UNSUP;

        const char *name = "Abhinav_ESP32_BLE";

        fields.name = (uint8_t *)name;
        fields.name_len = strlen(name);
        fields.name_is_complete = 1;

        ble_gap_adv_set_fields(&fields);

        struct ble_gap_adv_params adv_params = {0};

        adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
        adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

        ble_gap_adv_start(
            own_addr_type,
            NULL,
            BLE_HS_FOREVER,
            &adv_params,
            gap_event,
            NULL);
    }
    static void ble_on_sync(void)
    {
        ble_hs_id_infer_auto(0, &own_addr_type);

        start_advertising();

        printf("BLE Advertising Started\n");
    }

    static void host_task(void *param)
    {
        nimble_port_run();
        nimble_port_freertos_deinit();
    }

    void ble_manager_init(void)
    {
        nimble_port_init();

        ble_hs_cfg.sync_cb = ble_on_sync;

        ble_gatts_count_cfg(gatt_svcs);
        ble_gatts_add_svcs(gatt_svcs);

        nimble_port_freertos_init(host_task);
    }
