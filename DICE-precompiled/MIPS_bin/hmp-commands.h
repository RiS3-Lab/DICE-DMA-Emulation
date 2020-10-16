

{
.name       = "help|?",
.args_type  = "name:S?",
.params     = "[cmd]",
.help       = "show the help",
.mhandler.cmd = do_help_cmd,
},


{
.name       = "commit",
.args_type  = "device:B",
.params     = "device|all",
.help       = "commit changes to the disk images (if -snapshot is used) or backing files",
.mhandler.cmd = hmp_commit,
},


{
.name       = "q|quit",
.args_type  = "",
.params     = "",
.help       = "quit the emulator",
.mhandler.cmd = hmp_quit,
},


{
.name       = "block_resize",
.args_type  = "device:B,size:o",
.params     = "device size",
.help       = "resize a block image",
.mhandler.cmd = hmp_block_resize,
},


{
.name       = "block_stream",
.args_type  = "device:B,speed:o?,base:s?",
.params     = "device [speed [base]]",
.help       = "copy data from a backing file into a block device",
.mhandler.cmd = hmp_block_stream,
},


{
.name       = "block_job_set_speed",
.args_type  = "device:B,speed:o",
.params     = "device speed",
.help       = "set maximum speed for a background block operation",
.mhandler.cmd = hmp_block_job_set_speed,
},


{
.name       = "block_job_cancel",
.args_type  = "force:-f,device:B",
.params     = "[-f] device",
.help       = "stop an active background block operation (use -f"
"\n\t\t\t if the operation is currently paused)",
.mhandler.cmd = hmp_block_job_cancel,
},


{
.name       = "block_job_complete",
.args_type  = "device:B",
.params     = "device",
.help       = "stop an active background block operation",
.mhandler.cmd = hmp_block_job_complete,
},


{
.name       = "block_job_pause",
.args_type  = "device:B",
.params     = "device",
.help       = "pause an active background block operation",
.mhandler.cmd = hmp_block_job_pause,
},


{
.name       = "block_job_resume",
.args_type  = "device:B",
.params     = "device",
.help       = "resume a paused background block operation",
.mhandler.cmd = hmp_block_job_resume,
},


{
.name       = "eject",
.args_type  = "force:-f,device:B",
.params     = "[-f] device",
.help       = "eject a removable medium (use -f to force it)",
.mhandler.cmd = hmp_eject,
},


{
.name       = "drive_del",
.args_type  = "id:B",
.params     = "device",
.help       = "remove host block device",
.mhandler.cmd = hmp_drive_del,
},


{
.name       = "change",
.args_type  = "device:B,target:F,arg:s?",
.params     = "device filename [format]",
.help       = "change a removable medium, optional format",
.mhandler.cmd = hmp_change,
},


{
.name       = "screendump",
.args_type  = "filename:F",
.params     = "filename",
.help       = "save screen into PPM image 'filename'",
.mhandler.cmd = hmp_screendump,
},


{
.name       = "logfile",
.args_type  = "filename:F",
.params     = "filename",
.help       = "output logs to 'filename'",
.mhandler.cmd = hmp_logfile,
},


{
.name       = "trace-event",
.args_type  = "name:s,option:b",
.params     = "name on|off",
.help       = "changes status of a specific trace event",
.mhandler.cmd = hmp_trace_event,
},


#if defined(CONFIG_TRACE_SIMPLE)
{
.name       = "trace-file",
.args_type  = "op:s?,arg:F?",
.params     = "on|off|flush|set [arg]",
.help       = "open, close, or flush trace file, or set a new file name",
.mhandler.cmd = hmp_trace_file,
},

#endif

{
.name       = "log",
.args_type  = "items:s",
.params     = "item1[,...]",
.help       = "activate logging of the specified items",
.mhandler.cmd = hmp_log,
},


{
.name       = "savevm",
.args_type  = "name:s?",
.params     = "[tag|id]",
.help       = "save a VM snapshot. If no tag or id are provided, a new snapshot is created",
.mhandler.cmd = hmp_savevm,
},


{
.name       = "loadvm",
.args_type  = "name:s",
.params     = "tag|id",
.help       = "restore a VM snapshot from its tag or id",
.mhandler.cmd = hmp_loadvm,
.command_completion = loadvm_completion,
},


{
.name       = "delvm",
.args_type  = "name:s",
.params     = "tag|id",
.help       = "delete a VM snapshot from its tag or id",
.mhandler.cmd = hmp_delvm,
.command_completion = delvm_completion,
},


{
.name       = "singlestep",
.args_type  = "option:s?",
.params     = "[on|off]",
.help       = "run emulation in singlestep mode or switch to normal mode",
.mhandler.cmd = hmp_singlestep,
},


{
.name       = "stop",
.args_type  = "",
.params     = "",
.help       = "stop emulation",
.mhandler.cmd = hmp_stop,
},


{
.name       = "c|cont",
.args_type  = "",
.params     = "",
.help       = "resume emulation",
.mhandler.cmd = hmp_cont,
},


{
.name       = "system_wakeup",
.args_type  = "",
.params     = "",
.help       = "wakeup guest from suspend",
.mhandler.cmd = hmp_system_wakeup,
},


{
.name       = "gdbserver",
.args_type  = "device:s?",
.params     = "[device]",
.help       = "start gdbserver on given device (default 'tcp::1234'), stop with 'none'",
.mhandler.cmd = hmp_gdbserver,
},


{
.name       = "x",
.args_type  = "fmt:/,addr:l",
.params     = "/fmt addr",
.help       = "virtual memory dump starting at 'addr'",
.mhandler.cmd = hmp_memory_dump,
},


{
.name       = "xp",
.args_type  = "fmt:/,addr:l",
.params     = "/fmt addr",
.help       = "physical memory dump starting at 'addr'",
.mhandler.cmd = hmp_physical_memory_dump,
},


{
.name       = "p|print",
.args_type  = "fmt:/,val:l",
.params     = "/fmt expr",
.help       = "print expression value (use $reg for CPU register access)",
.mhandler.cmd = do_print,
},


{
.name       = "i",
.args_type  = "fmt:/,addr:i,index:i.",
.params     = "/fmt addr",
.help       = "I/O port read",
.mhandler.cmd = hmp_ioport_read,
},


{
.name       = "o",
.args_type  = "fmt:/,addr:i,val:i",
.params     = "/fmt addr value",
.help       = "I/O port write",
.mhandler.cmd = hmp_ioport_write,
},


{
.name       = "sendkey",
.args_type  = "keys:s,hold-time:i?",
.params     = "keys [hold_ms]",
.help       = "send keys to the VM (e.g. 'sendkey ctrl-alt-f1', default hold time=100 ms)",
.mhandler.cmd = hmp_sendkey,
.command_completion = sendkey_completion,
},


{
.name       = "system_reset",
.args_type  = "",
.params     = "",
.help       = "reset the system",
.mhandler.cmd = hmp_system_reset,
},


{
.name       = "system_powerdown",
.args_type  = "",
.params     = "",
.help       = "send system power down event",
.mhandler.cmd = hmp_system_powerdown,
},


{
.name       = "sum",
.args_type  = "start:i,size:i",
.params     = "addr size",
.help       = "compute the checksum of a memory region",
.mhandler.cmd = hmp_sum,
},


{
.name       = "usb_add",
.args_type  = "devname:s",
.params     = "device",
.help       = "add USB device (e.g. 'host:bus.addr' or 'host:vendor_id:product_id')",
.mhandler.cmd = hmp_usb_add,
},


{
.name       = "usb_del",
.args_type  = "devname:s",
.params     = "device",
.help       = "remove USB device 'bus.addr'",
.mhandler.cmd = hmp_usb_del,
},


{
.name       = "device_add",
.args_type  = "device:O",
.params     = "driver[,prop=value][,...]",
.help       = "add device, like -device on the command line",
.mhandler.cmd = hmp_device_add,
.command_completion = device_add_completion,
},


{
.name       = "device_del",
.args_type  = "id:s",
.params     = "device",
.help       = "remove device",
.mhandler.cmd = hmp_device_del,
.command_completion = device_del_completion,
},


{
.name       = "cpu",
.args_type  = "index:i",
.params     = "index",
.help       = "set the default CPU",
.mhandler.cmd = hmp_cpu,
},


{
.name       = "mouse_move",
.args_type  = "dx_str:s,dy_str:s,dz_str:s?",
.params     = "dx dy [dz]",
.help       = "send mouse move events",
.mhandler.cmd = hmp_mouse_move,
},


{
.name       = "mouse_button",
.args_type  = "button_state:i",
.params     = "state",
.help       = "change mouse button state (1=L, 2=M, 4=R)",
.mhandler.cmd = hmp_mouse_button,
},


{
.name       = "mouse_set",
.args_type  = "index:i",
.params     = "index",
.help       = "set which mouse device receives events",
.mhandler.cmd = hmp_mouse_set,
},


{
.name       = "wavcapture",
.args_type  = "path:F,freq:i?,bits:i?,nchannels:i?",
.params     = "path [frequency [bits [channels]]]",
.help       = "capture audio to a wave file (default frequency=44100 bits=16 channels=2)",
.mhandler.cmd = hmp_wavcapture,
},

{
.name       = "stopcapture",
.args_type  = "n:i",
.params     = "capture index",
.help       = "stop capture",
.mhandler.cmd = hmp_stopcapture,
},

{
.name       = "memsave",
.args_type  = "val:l,size:i,filename:s",
.params     = "addr size file",
.help       = "save to disk virtual memory dump starting at 'addr' of size 'size'",
.mhandler.cmd = hmp_memsave,
},


{
.name       = "pmemsave",
.args_type  = "val:l,size:i,filename:s",
.params     = "addr size file",
.help       = "save to disk physical memory dump starting at 'addr' of size 'size'",
.mhandler.cmd = hmp_pmemsave,
},


{
.name       = "boot_set",
.args_type  = "bootdevice:s",
.params     = "bootdevice",
.help       = "define new values for the boot device list",
.mhandler.cmd = hmp_boot_set,
},


{
.name       = "nmi",
.args_type  = "",
.params     = "",
.help       = "inject an NMI",
.mhandler.cmd = hmp_nmi,
},

{
.name       = "ringbuf_write",
.args_type  = "device:s,data:s",
.params     = "device data",
.help       = "Write to a ring buffer character device",
.mhandler.cmd = hmp_ringbuf_write,
.command_completion = ringbuf_write_completion,
},


{
.name       = "ringbuf_read",
.args_type  = "device:s,size:i",
.params     = "device size",
.help       = "Read from a ring buffer character device",
.mhandler.cmd = hmp_ringbuf_read,
.command_completion = ringbuf_write_completion,
},


{
.name       = "migrate",
.args_type  = "detach:-d,blk:-b,inc:-i,uri:s",
.params     = "[-d] [-b] [-i] uri",
.help       = "migrate to URI (using -d to not wait for completion)"
"\n\t\t\t -b for migration without shared storage with"
" full copy of disk\n\t\t\t -i for migration without "
"shared storage with incremental copy of disk "
"(base image shared between src and destination)",
.mhandler.cmd = hmp_migrate,
},



{
.name       = "migrate_cancel",
.args_type  = "",
.params     = "",
.help       = "cancel the current VM migration",
.mhandler.cmd = hmp_migrate_cancel,
},


{
.name       = "migrate_incoming",
.args_type  = "uri:s",
.params     = "uri",
.help       = "Continue an incoming migration from an -incoming defer",
.mhandler.cmd = hmp_migrate_incoming,
},


{
.name       = "migrate_set_cache_size",
.args_type  = "value:o",
.params     = "value",
.help       = "set cache size (in bytes) for XBZRLE migrations,"
"the cache size will be rounded down to the nearest "
"power of 2.\n"
"The cache size affects the number of cache misses."
"In case of a high cache miss ratio you need to increase"
" the cache size",
.mhandler.cmd = hmp_migrate_set_cache_size,
},


{
.name       = "migrate_set_speed",
.args_type  = "value:o",
.params     = "value",
.help       = "set maximum speed (in bytes) for migrations. "
"Defaults to MB if no size suffix is specified, ie. B/K/M/G/T",
.mhandler.cmd = hmp_migrate_set_speed,
},


{
.name       = "migrate_set_downtime",
.args_type  = "value:T",
.params     = "value",
.help       = "set maximum tolerated downtime (in seconds) for migrations",
.mhandler.cmd = hmp_migrate_set_downtime,
},


{
.name       = "migrate_set_capability",
.args_type  = "capability:s,state:b",
.params     = "capability state",
.help       = "Enable/Disable the usage of a capability for migration",
.mhandler.cmd = hmp_migrate_set_capability,
.command_completion = migrate_set_capability_completion,
},


{
.name       = "migrate_set_parameter",
.args_type  = "parameter:s,value:i",
.params     = "parameter value",
.help       = "Set the parameter for migration",
.mhandler.cmd = hmp_migrate_set_parameter,
.command_completion = migrate_set_parameter_completion,
},


{
.name       = "client_migrate_info",
.args_type  = "protocol:s,hostname:s,port:i?,tls-port:i?,cert-subject:s?",
.params     = "protocol hostname port tls-port cert-subject",
.help       = "set migration information for remote display",
.mhandler.cmd = hmp_client_migrate_info,
},


{
.name       = "dump-guest-memory",
.args_type  = "paging:-p,zlib:-z,lzo:-l,snappy:-s,filename:F,begin:i?,length:i?",
.params     = "[-p] [-z|-l|-s] filename [begin length]",
.help       = "dump guest memory into file 'filename'.\n\t\t\t"
"-p: do paging to get guest's memory mapping.\n\t\t\t"
"-z: dump in kdump-compressed format, with zlib compression.\n\t\t\t"
"-l: dump in kdump-compressed format, with lzo compression.\n\t\t\t"
"-s: dump in kdump-compressed format, with snappy compression.\n\t\t\t"
"begin: the starting physical address.\n\t\t\t"
"length: the memory size, in bytes.",
.mhandler.cmd = hmp_dump_guest_memory,
},



{
.name       = "snapshot_blkdev",
.args_type  = "reuse:-n,device:B,snapshot-file:s?,format:s?",
.params     = "[-n] device [new-image-file] [format]",
.help       = "initiates a live snapshot\n\t\t\t"
"of device. If a new image file is specified, the\n\t\t\t"
"new image file will become the new root image.\n\t\t\t"
"If format is specified, the snapshot file will\n\t\t\t"
"be created in that format.\n\t\t\t"
"The default format is qcow2.  The -n flag requests QEMU\n\t\t\t"
"to reuse the image found in new-image-file, instead of\n\t\t\t"
"recreating it from scratch.",
.mhandler.cmd = hmp_snapshot_blkdev,
},


{
.name       = "snapshot_blkdev_internal",
.args_type  = "device:B,name:s",
.params     = "device name",
.help       = "take an internal snapshot of device.\n\t\t\t"
"The format of the image used by device must\n\t\t\t"
"support it, such as qcow2.\n\t\t\t",
.mhandler.cmd = hmp_snapshot_blkdev_internal,
},


{
.name       = "snapshot_delete_blkdev_internal",
.args_type  = "device:B,name:s,id:s?",
.params     = "device name [id]",
.help       = "delete an internal snapshot of device.\n\t\t\t"
"If id is specified, qemu will try delete\n\t\t\t"
"the snapshot matching both id and name.\n\t\t\t"
"The format of the image used by device must\n\t\t\t"
"support it, such as qcow2.\n\t\t\t",
.mhandler.cmd = hmp_snapshot_delete_blkdev_internal,
},


{
.name       = "drive_mirror",
.args_type  = "reuse:-n,full:-f,device:B,target:s,format:s?",
.params     = "[-n] [-f] device target [format]",
.help       = "initiates live storage\n\t\t\t"
"migration for a device. The device's contents are\n\t\t\t"
"copied to the new image file, including data that\n\t\t\t"
"is written after the command is started.\n\t\t\t"
"The -n flag requests QEMU to reuse the image found\n\t\t\t"
"in new-image-file, instead of recreating it from scratch.\n\t\t\t"
"The -f flag requests QEMU to copy the whole disk,\n\t\t\t"
"so that the result does not need a backing file.\n\t\t\t",
.mhandler.cmd = hmp_drive_mirror,
},

{
.name       = "drive_backup",
.args_type  = "reuse:-n,full:-f,device:B,target:s,format:s?",
.params     = "[-n] [-f] device target [format]",
.help       = "initiates a point-in-time\n\t\t\t"
"copy for a device. The device's contents are\n\t\t\t"
"copied to the new image file, excluding data that\n\t\t\t"
"is written after the command is started.\n\t\t\t"
"The -n flag requests QEMU to reuse the image found\n\t\t\t"
"in new-image-file, instead of recreating it from scratch.\n\t\t\t"
"The -f flag requests QEMU to copy the whole disk,\n\t\t\t"
"so that the result does not need a backing file.\n\t\t\t",
.mhandler.cmd = hmp_drive_backup,
},

{
.name       = "drive_add",
.args_type  = "pci_addr:s,opts:s",
.params     = "[[<domain>:]<bus>:]<slot>\n"
"[file=file][,if=type][,bus=n]\n"
"[,unit=m][,media=d][,index=i]\n"
"[,cyls=c,heads=h,secs=s[,trans=t]]\n"
"[,snapshot=on|off][,cache=on|off]\n"
"[,readonly=on|off][,copy-on-read=on|off]",
.help       = "add drive to PCI storage controller",
.mhandler.cmd = hmp_drive_add,
},


{
.name       = "pcie_aer_inject_error",
.args_type  = "advisory_non_fatal:-a,correctable:-c,"
"id:s,error_status:s,"
"header0:i?,header1:i?,header2:i?,header3:i?,"
"prefix0:i?,prefix1:i?,prefix2:i?,prefix3:i?",
.params     = "[-a] [-c] id "
"<error_status> [<tlp header> [<tlp header prefix>]]",
.help       = "inject pcie aer error\n\t\t\t"
" -a for advisory non fatal error\n\t\t\t"
" -c for correctable error\n\t\t\t"
"<id> = qdev device id\n\t\t\t"
"<error_status> = error string or 32bit\n\t\t\t"
"<tlb header> = 32bit x 4\n\t\t\t"
"<tlb header prefix> = 32bit x 4",
.mhandler.cmd = hmp_pcie_aer_inject_error,
},


{
.name       = "host_net_add",
.args_type  = "device:s,opts:s?",
.params     = "tap|user|socket|vde|netmap|bridge|vhost-user|dump [options]",
.help       = "add host VLAN client",
.mhandler.cmd = hmp_host_net_add,
.command_completion = host_net_add_completion,
},


{
.name       = "host_net_remove",
.args_type  = "vlan_id:i,device:s",
.params     = "vlan_id name",
.help       = "remove host VLAN client",
.mhandler.cmd = hmp_host_net_remove,
.command_completion = host_net_remove_completion,
},


{
.name       = "netdev_add",
.args_type  = "netdev:O",
.params     = "[user|tap|socket|vde|bridge|hubport|netmap|vhost-user],id=str[,prop=value][,...]",
.help       = "add host network device",
.mhandler.cmd = hmp_netdev_add,
.command_completion = netdev_add_completion,
},


{
.name       = "netdev_del",
.args_type  = "id:s",
.params     = "id",
.help       = "remove host network device",
.mhandler.cmd = hmp_netdev_del,
.command_completion = netdev_del_completion,
},


{
.name       = "object_add",
.args_type  = "object:O",
.params     = "[qom-type=]type,id=str[,prop=value][,...]",
.help       = "create QOM object",
.mhandler.cmd = hmp_object_add,
.command_completion = object_add_completion,
},


{
.name       = "object_del",
.args_type  = "id:s",
.params     = "id",
.help       = "destroy QOM object",
.mhandler.cmd = hmp_object_del,
.command_completion = object_del_completion,
},


#ifdef CONFIG_SLIRP
{
.name       = "hostfwd_add",
.args_type  = "arg1:s,arg2:s?,arg3:s?",
.params     = "[vlan_id name] [tcp|udp]:[hostaddr]:hostport-[guestaddr]:guestport",
.help       = "redirect TCP or UDP connections from host to guest (requires -net user)",
.mhandler.cmd = hmp_hostfwd_add,
},
#endif

#ifdef CONFIG_SLIRP
{
.name       = "hostfwd_remove",
.args_type  = "arg1:s,arg2:s?,arg3:s?",
.params     = "[vlan_id name] [tcp|udp]:[hostaddr]:hostport",
.help       = "remove host-to-guest TCP or UDP redirection",
.mhandler.cmd = hmp_hostfwd_remove,
},

#endif

{
.name       = "balloon",
.args_type  = "value:M",
.params     = "target",
.help       = "request VM to change its memory allocation (in MB)",
.mhandler.cmd = hmp_balloon,
},


{
.name       = "set_link",
.args_type  = "name:s,up:b",
.params     = "name on|off",
.help       = "change the link status of a network adapter",
.mhandler.cmd = hmp_set_link,
.command_completion = set_link_completion,
},


{
.name       = "watchdog_action",
.args_type  = "action:s",
.params     = "[reset|shutdown|poweroff|pause|debug|none]",
.help       = "change watchdog action",
.mhandler.cmd = hmp_watchdog_action,
.command_completion = watchdog_action_completion,
},


{
.name       = "acl_show",
.args_type  = "aclname:s",
.params     = "aclname",
.help       = "list rules in the access control list",
.mhandler.cmd = hmp_acl_show,
},


{
.name       = "acl_policy",
.args_type  = "aclname:s,policy:s",
.params     = "aclname allow|deny",
.help       = "set default access control list policy",
.mhandler.cmd = hmp_acl_policy,
},


{
.name       = "acl_add",
.args_type  = "aclname:s,match:s,policy:s,index:i?",
.params     = "aclname match allow|deny [index]",
.help       = "add a match rule to the access control list",
.mhandler.cmd = hmp_acl_add,
},


{
.name       = "acl_remove",
.args_type  = "aclname:s,match:s",
.params     = "aclname match",
.help       = "remove a match rule from the access control list",
.mhandler.cmd = hmp_acl_remove,
},


{
.name       = "acl_reset",
.args_type  = "aclname:s",
.params     = "aclname",
.help       = "reset the access control list",
.mhandler.cmd = hmp_acl_reset,
},


{
.name       = "nbd_server_start",
.args_type  = "all:-a,writable:-w,uri:s",
.params     = "nbd_server_start [-a] [-w] host:port",
.help       = "serve block devices on the given host and port",
.mhandler.cmd = hmp_nbd_server_start,
},

{
.name       = "nbd_server_add",
.args_type  = "writable:-w,device:B",
.params     = "nbd_server_add [-w] device",
.help       = "export a block device via NBD",
.mhandler.cmd = hmp_nbd_server_add,
},

{
.name       = "nbd_server_stop",
.args_type  = "",
.params     = "nbd_server_stop",
.help       = "stop serving block devices using the NBD protocol",
.mhandler.cmd = hmp_nbd_server_stop,
},


#if defined(TARGET_I386)

{
.name       = "mce",
.args_type  = "broadcast:-b,cpu_index:i,bank:i,status:l,mcg_status:l,addr:l,misc:l",
.params     = "[-b] cpu bank status mcgstatus addr misc",
.help       = "inject a MCE on the given CPU [and broadcast to other CPUs with -b option]",
.mhandler.cmd = hmp_mce,
},

#endif

{
.name       = "getfd",
.args_type  = "fdname:s",
.params     = "getfd name",
.help       = "receive a file descriptor via SCM rights and assign it a name",
.mhandler.cmd = hmp_getfd,
},


{
.name       = "closefd",
.args_type  = "fdname:s",
.params     = "closefd name",
.help       = "close a file descriptor previously passed via SCM rights",
.mhandler.cmd = hmp_closefd,
},


{
.name       = "block_passwd",
.args_type  = "device:B,password:s",
.params     = "block_passwd device password",
.help       = "set the password of encrypted block devices",
.mhandler.cmd = hmp_block_passwd,
},


{
.name       = "block_set_io_throttle",
.args_type  = "device:B,bps:l,bps_rd:l,bps_wr:l,iops:l,iops_rd:l,iops_wr:l",
.params     = "device bps bps_rd bps_wr iops iops_rd iops_wr",
.help       = "change I/O throttle limits for a block drive",
.mhandler.cmd = hmp_block_set_io_throttle,
},


{
.name       = "set_password",
.args_type  = "protocol:s,password:s,connected:s?",
.params     = "protocol password action-if-connected",
.help       = "set spice/vnc password",
.mhandler.cmd = hmp_set_password,
},


{
.name       = "expire_password",
.args_type  = "protocol:s,time:s",
.params     = "protocol time",
.help       = "set spice/vnc password expire-time",
.mhandler.cmd = hmp_expire_password,
},


{
.name       = "chardev-add",
.args_type  = "args:s",
.params     = "args",
.help       = "add chardev",
.mhandler.cmd = hmp_chardev_add,
.command_completion = chardev_add_completion,
},


{
.name       = "chardev-remove",
.args_type  = "id:s",
.params     = "id",
.help       = "remove chardev",
.mhandler.cmd = hmp_chardev_remove,
.command_completion = chardev_remove_completion,
},


{
.name       = "qemu-io",
.args_type  = "device:B,command:s",
.params     = "[device] \"[command]\"",
.help       = "run a qemu-io command on a block device",
.mhandler.cmd = hmp_qemu_io,
},


{
.name       = "cpu-add",
.args_type  = "id:i",
.params     = "id",
.help       = "add cpu",
.mhandler.cmd  = hmp_cpu_add,
},


{
.name       = "qom-list",
.args_type  = "path:s?",
.params     = "path",
.help       = "list QOM properties",
.mhandler.cmd  = hmp_qom_list,
},


{
.name       = "qom-set",
.args_type  = "path:s,property:s,value:s",
.params     = "path property value",
.help       = "set QOM property",
.mhandler.cmd  = hmp_qom_set,
},


{
.name       = "info",
.args_type  = "item:s?",
.params     = "[subcommand]",
.help       = "show various information about the system state",
.mhandler.cmd = hmp_info_help,
.sub_table = info_cmds,
},







