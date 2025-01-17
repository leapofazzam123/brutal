#include <brutal/log.h>
#include <virtio/device.h>

void virtio_device_set_if_acknowledged(VirtioDevice *device, uint8_t status)
{
    assert_truth(virtio_in_status(device, VIRTIO_STATUS_ACKNOWLEDGE));
    device->device_status |= status;
}

void virtio_device_clear_status(VirtioDevice *device)
{
    device->device_status = 0;
}

bool virtio_in_status(VirtioDevice *device, uint8_t status)
{
    return device->device_status & status;
}

VirtioDeviceResult virtio_set_status(VirtioDevice *device, uint8_t status)
{
    switch (status)
    {
    case VIRTIO_STATUS_ACKNOWLEDGE:
        device->device_status |= status;
        return VIRTIO_DEVICE_OK;

    case VIRTIO_STATUS_DRIVER_AVAILABLE:
        virtio_device_set_if_acknowledged(device, status);
        return VIRTIO_DEVICE_OK;

    case VIRTIO_STATUS_FAILURE:
        device->device_status |= status;
        return VIRTIO_DEVICE_OK;

    case VIRTIO_STATUS_FEATURES_OK:
        virtio_device_set_if_acknowledged(device, status);
        return VIRTIO_DEVICE_OK;

    case VIRTIO_STATUS_DRIVER_OK:
        virtio_device_set_if_acknowledged(device, status);
        return VIRTIO_DEVICE_OK;

    case VIRTIO_STATUS_NEEDS_RESET:
        device->device_status |= status;
        return VIRTIO_DEVICE_OK;

    default:
        return VIRTIO_DEVICE_UNKNOWN_STATUS;
    }

    assert_unreachable();
}

VirtioDeviceResult virtio_device_reset(VirtioDevice *device)
{
    VirtioDeviceResult result = virtio_set_status(device, VIRTIO_STATUS_NEEDS_RESET);

    while (virtio_in_status(device, VIRTIO_STATUS_NEEDS_RESET))
        continue;

    return result;
}

VirtioDeviceResult virtio_device_init(VirtioDevice *device, VirtioDeviceInit *device_init)
{
    VirtioDeviceResult result;

    // Step 1: Reset device
    result = virtio_device_reset(device);

    // Step 2: Acknowledge it
    result = virtio_set_status(device, VIRTIO_STATUS_ACKNOWLEDGE);

    // Step 3: Set the DRIVER_AVAILABLE status
    result = virtio_set_status(device, VIRTIO_STATUS_DRIVER_AVAILABLE);

    // Step 4: Negociate the features
    result = virtio_device_negociate_features(device, device_init);

    // Step 5: Set the FEATURES_OK status
    result = virtio_set_status(device, VIRTIO_STATUS_FEATURES_OK);

    // Step 6: Check that FEATURES_OK is still set.
    // FIXME: Maybe the bit is not set at this moment
    if (!virtio_in_status(device, VIRTIO_STATUS_FEATURES_OK))
        return VIRTIO_DEVICE_FEATURES_NEGOCIATE_FAILED;

    return result;
}

VirtioDeviceResult virtio_device_negociate_features(VirtioDevice *device, VirtioDeviceInit *init)
{
    uint64_t device_features = device->device_features;
    uint64_t needed_features = init->needed_features;

    // In this case the device doesn't have all the features needed by the driver
    if ((device_features & needed_features) == needed_features)
    {
        virtio_set_status(device, VIRTIO_STATUS_FAILURE);
        return VIRTIO_DEVICE_FEATURES_NEGOCIATE_FAILED;
    }

    // Else the driver's negociations went well
    return VIRTIO_DEVICE_OK;
}