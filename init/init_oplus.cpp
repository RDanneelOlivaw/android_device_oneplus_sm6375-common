/*
 * Copyright (C) 2022-2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;

/*
 * SetProperty does not allow updating read only properties and as a result
 * does not work for our use case. Write "OverrideProperty" to do practically
 * the same thing as "SetProperty" without this restriction.
 */
void OverrideProperty(const char* name, const char* value) {
    size_t valuelen = strlen(value);

    prop_info* pi = (prop_info*)__system_property_find(name);
    if (pi != nullptr) {
        __system_property_update(pi, value, valuelen);
    } else {
        __system_property_add(name, strlen(name), value, valuelen);
    }
}

/*
 * Only for read-only properties. Properties that can be wrote to more
 * than once should be set in a typical init script (e.g. init.oplus.hw.rc)
 * after the original property has been set.
 */
void vendor_load_properties() {
    auto hw_region_id = std::stoi(GetProperty("ro.boot.hw_region_id", "0"));
    auto prjname = std::stoi(GetProperty("ro.boot.prjname", "0"));

    switch (hw_region_id) {
        // larry
        case 2: // IN
            OverrideProperty("ro.product.product.model", "CPH2467");
            break;
        case 3: // EU
            OverrideProperty("ro.product.product.model", "CPH2465");
            break;
        case 4: // NA
            OverrideProperty("ro.product.product.model", "CPH2513");
            break;
        default:
            LOG(ERROR) << "Unexpected region ID: " << hw_region_id;
    }

    switch (prjname) {
        // gunnar
        case 20826: // T-Mobile (20826)
            OverrideProperty("ro.product.product.model", "GN2200");
            break;
        default:
            LOG(ERROR) << "Unexpected project name: " << prjname;
    }
}
