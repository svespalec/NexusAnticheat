#include "driver.hpp"

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    // Set our driver unload routine
    DriverObject->DriverUnload = DriverUnload;

    DbgPrint("NexusDriver: Loaded\n");
    return STATUS_SUCCESS;
}

void DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("NexusDriver: Unloaded\n");
}
