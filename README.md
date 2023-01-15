# local_time_info
Modifications to NCS API's to access local time info from Current Time Service

To update your version of NCS with API's to enable you to access the Local Time information from the BLE Current Time Service:
1. Copy the two .patch files into the /nrf directory of your NCS install directory e.g. ncs/v2.x.x/nrf
2. Navigate to the /nrf directory and apply the patches using GitBash:
    a.	$ git apply cts_client_c.patch
    b.	$ git apply cts_client_h.patch

This will update the cts_client.c and cts_client.h with modified API's.

The mods will add an additional API for reading the local time info:

    int bt_cts_read_local_time(struct bt_cts_client *cts_c, bt_lts_read_cb func)

It also includes a modification to the API for assigning handles to the CTS to include a flag that 
indicates whether the local time info is available or not:

    int bt_cts_handles_assign(struct bt_gatt_dm *dm, struct bt_cts_client *cts_c, int *flag);

You can use this flag to determine whether you check the local time info or not once connected to the CTS




