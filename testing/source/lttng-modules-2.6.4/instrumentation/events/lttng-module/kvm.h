#if !defined(LTTNG_TRACE_KVM_MAIN_H) || defined(TRACE_HEADER_MULTI_READ)
#define LTTNG_TRACE_KVM_MAIN_H

#include "../../../probes/lttng-tracepoint-event.h"
#include <linux/version.h>

#undef TRACE_SYSTEM
#define TRACE_SYSTEM kvm

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,38))

#define ERSN(x) { KVM_EXIT_##x, "KVM_EXIT_" #x }

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0))

#define kvm_trace_exit_reason						\
	ERSN(UNKNOWN), ERSN(EXCEPTION), ERSN(IO), ERSN(HYPERCALL),	\
	ERSN(DEBUG), ERSN(HLT), ERSN(MMIO), ERSN(IRQ_WINDOW_OPEN),	\
	ERSN(SHUTDOWN), ERSN(FAIL_ENTRY), ERSN(INTR), ERSN(SET_TPR),	\
	ERSN(TPR_ACCESS), ERSN(S390_SIEIC), ERSN(S390_RESET), ERSN(DCR),\
	ERSN(NMI), ERSN(INTERNAL_ERROR), ERSN(OSI), ERSN(PAPR_HCALL),	\
	ERSN(S390_UCONTROL), ERSN(WATCHDOG), ERSN(S390_TSCH)

#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(3,6,0))

#define kvm_trace_exit_reason						\
	ERSN(UNKNOWN), ERSN(EXCEPTION), ERSN(IO), ERSN(HYPERCALL),	\
	ERSN(DEBUG), ERSN(HLT), ERSN(MMIO), ERSN(IRQ_WINDOW_OPEN),	\
	ERSN(SHUTDOWN), ERSN(FAIL_ENTRY), ERSN(INTR), ERSN(SET_TPR),	\
	ERSN(TPR_ACCESS), ERSN(S390_SIEIC), ERSN(S390_RESET), ERSN(DCR),\
	ERSN(NMI), ERSN(INTERNAL_ERROR), ERSN(OSI), ERSN(PAPR_HCALL),	\
	ERSN(S390_UCONTROL)

#else

#define kvm_trace_exit_reason						\
	ERSN(UNKNOWN), ERSN(EXCEPTION), ERSN(IO), ERSN(HYPERCALL),	\
	ERSN(DEBUG), ERSN(HLT), ERSN(MMIO), ERSN(IRQ_WINDOW_OPEN),	\
	ERSN(SHUTDOWN), ERSN(FAIL_ENTRY), ERSN(INTR), ERSN(SET_TPR),	\
	ERSN(TPR_ACCESS), ERSN(S390_SIEIC), ERSN(S390_RESET), ERSN(DCR),\
	ERSN(NMI), ERSN(INTERNAL_ERROR), ERSN(OSI)

#endif

LTTNG_TRACEPOINT_EVENT(kvm_userspace_exit,
	    TP_PROTO(__u32 reason, int errno),
	    TP_ARGS(reason, errno),

	TP_STRUCT__entry(
		__field(	__u32,		reason		)
		__field(	int,		errno		)
	),

	TP_fast_assign(
		tp_assign(reason, reason)
		tp_assign(errno, errno)
	),

	TP_printk("reason %s (%d)",
		  __entry->errno < 0 ?
		  (__entry->errno == -EINTR ? "restart" : "error") :
		  __print_symbolic(__entry->reason, kvm_trace_exit_reason),
		  __entry->errno < 0 ? -__entry->errno : __entry->reason)
)
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,6,0))
#if defined(__KVM_HAVE_IOAPIC)
#undef __KVM_HAVE_IRQ_LINE
#define __KVM_HAVE_IRQ_LINE
#endif
#endif

#if defined(__KVM_HAVE_IRQ_LINE)
LTTNG_TRACEPOINT_EVENT(kvm_set_irq,
	TP_PROTO(unsigned int gsi, int level, int irq_source_id),
	TP_ARGS(gsi, level, irq_source_id),

	TP_STRUCT__entry(
		__field(	unsigned int,	gsi		)
		__field(	int,		level		)
		__field(	int,		irq_source_id	)
	),

	TP_fast_assign(
		tp_assign(gsi, gsi)
		tp_assign(level, level)
		tp_assign(irq_source_id, irq_source_id)
	),

	TP_printk("gsi %u level %d source %d",
		  __entry->gsi, __entry->level, __entry->irq_source_id)
)
#endif

#if defined(__KVM_HAVE_IOAPIC)
#define kvm_deliver_mode		\
	{0x0, "Fixed"},			\
	{0x1, "LowPrio"},		\
	{0x2, "SMI"},			\
	{0x3, "Res3"},			\
	{0x4, "NMI"},			\
	{0x5, "INIT"},			\
	{0x6, "SIPI"},			\
	{0x7, "ExtINT"}

LTTNG_TRACEPOINT_EVENT(kvm_ioapic_set_irq,
	    TP_PROTO(__u64 e, int pin, bool coalesced),
	    TP_ARGS(e, pin, coalesced),

	TP_STRUCT__entry(
		__field(	__u64,		e		)
		__field(	int,		pin		)
		__field(	bool,		coalesced	)
	),

	TP_fast_assign(
		tp_assign(e, e)
		tp_assign(pin, pin)
		tp_assign(coalesced, coalesced)
	),

	TP_printk("pin %u dst %x vec=%u (%s|%s|%s%s)%s",
		  __entry->pin, (u8)(__entry->e >> 56), (u8)__entry->e,
		  __print_symbolic((__entry->e >> 8 & 0x7), kvm_deliver_mode),
		  (__entry->e & (1<<11)) ? "logical" : "physical",
		  (__entry->e & (1<<15)) ? "level" : "edge",
		  (__entry->e & (1<<16)) ? "|masked" : "",
		  __entry->coalesced ? " (coalesced)" : "")
)

LTTNG_TRACEPOINT_EVENT(kvm_msi_set_irq,
	    TP_PROTO(__u64 address, __u64 data),
	    TP_ARGS(address, data),

	TP_STRUCT__entry(
		__field(	__u64,		address		)
		__field(	__u64,		data		)
	),

	TP_fast_assign(
		tp_assign(address, address)
		tp_assign(data, data)
	),

	TP_printk("dst %u vec %x (%s|%s|%s%s)",
		  (u8)(__entry->address >> 12), (u8)__entry->data,
		  __print_symbolic((__entry->data >> 8 & 0x7), kvm_deliver_mode),
		  (__entry->address & (1<<2)) ? "logical" : "physical",
		  (__entry->data & (1<<15)) ? "level" : "edge",
		  (__entry->address & (1<<3)) ? "|rh" : "")
)

#define kvm_irqchips						\
	{KVM_IRQCHIP_PIC_MASTER,	"PIC master"},		\
	{KVM_IRQCHIP_PIC_SLAVE,		"PIC slave"},		\
	{KVM_IRQCHIP_IOAPIC,		"IOAPIC"}

LTTNG_TRACEPOINT_EVENT(kvm_ack_irq,
	TP_PROTO(unsigned int irqchip, unsigned int pin),
	TP_ARGS(irqchip, pin),

	TP_STRUCT__entry(
		__field(	unsigned int,	irqchip		)
		__field(	unsigned int,	pin		)
	),

	TP_fast_assign(
		tp_assign(irqchip, irqchip)
		tp_assign(pin, pin)
	),

	TP_printk("irqchip %s pin %u",
		  __print_symbolic(__entry->irqchip, kvm_irqchips),
		 __entry->pin)
)



#endif /* defined(__KVM_HAVE_IOAPIC) */

#define KVM_TRACE_MMIO_READ_UNSATISFIED 0
#define KVM_TRACE_MMIO_READ 1
#define KVM_TRACE_MMIO_WRITE 2

#define kvm_trace_symbol_mmio \
	{ KVM_TRACE_MMIO_READ_UNSATISFIED, "unsatisfied-read" }, \
	{ KVM_TRACE_MMIO_READ, "read" }, \
	{ KVM_TRACE_MMIO_WRITE, "write" }

LTTNG_TRACEPOINT_EVENT(kvm_mmio,
	TP_PROTO(int type, int len, u64 gpa, u64 val),
	TP_ARGS(type, len, gpa, val),

	TP_STRUCT__entry(
		__field(	u32,	type		)
		__field(	u32,	len		)
		__field(	u64,	gpa		)
		__field(	u64,	val		)
	),

	TP_fast_assign(
		tp_assign(type, type)
		tp_assign(len, len)
		tp_assign(gpa, gpa)
		tp_assign(val, val)
	),

	TP_printk("mmio %s len %u gpa 0x%llx val 0x%llx",
		  __print_symbolic(__entry->type, kvm_trace_symbol_mmio),
		  __entry->len, __entry->gpa, __entry->val)
)

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,34))

#define kvm_fpu_load_symbol	\
	{0, "unload"},		\
	{1, "load"}

LTTNG_TRACEPOINT_EVENT(kvm_fpu,
	TP_PROTO(int load),
	TP_ARGS(load),

	TP_STRUCT__entry(
		__field(	u32,	        load		)
	),

	TP_fast_assign(
		tp_assign(load, load)
	),

	TP_printk("%s", __print_symbolic(__entry->load, kvm_fpu_load_symbol))
)

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,18,0))

LTTNG_TRACEPOINT_EVENT(kvm_age_page,
	TP_PROTO(ulong gfn, int level, struct kvm_memory_slot *slot, int ref),
	TP_ARGS(gfn, level, slot, ref),

	TP_STRUCT__entry(
		__field(        u64,    hva             )
		__field(        u64,    gfn             )
		__field(        u8,     level           )
		__field(        u8,     referenced      )
	),

	TP_fast_assign(
		tp_assign(gfn, gfn)
		tp_assign(level, level)
		tp_assign(hva, ((gfn - slot->base_gfn) <<
			PAGE_SHIFT) + slot->userspace_addr)
		tp_assign(referenced, ref)
	),

	TP_printk("hva %llx gfn %llx level %u %s",
		__entry->hva, __entry->gfn, __entry->level,
		__entry->referenced ? "YOUNG" : "OLD")
)

#else

LTTNG_TRACEPOINT_EVENT(kvm_age_page,
	TP_PROTO(ulong hva, struct kvm_memory_slot *slot, int ref),
	TP_ARGS(hva, slot, ref),

	TP_STRUCT__entry(
		__field(	u64,	hva		)
		__field(	u64,	gfn		)
		__field(	u8,	referenced	)
	),

	TP_fast_assign(
		tp_assign(hva, hva)
		tp_assign(gfn,
		  slot->base_gfn + ((hva - slot->userspace_addr) >> PAGE_SHIFT))
		tp_assign(referenced, ref)
	),

	TP_printk("hva %llx gfn %llx %s",
		  __entry->hva, __entry->gfn,
		  __entry->referenced ? "YOUNG" : "OLD")
)
#endif
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,38))

#ifdef CONFIG_KVM_ASYNC_PF
LTTNG_TRACEPOINT_EVENT_CLASS(kvm_async_get_page_class,

	TP_PROTO(u64 gva, u64 gfn),

	TP_ARGS(gva, gfn),

	TP_STRUCT__entry(
		__field(__u64, gva)
		__field(u64, gfn)
	),

	TP_fast_assign(
		tp_assign(gva, gva)
		tp_assign(gfn, gfn)
	),

	TP_printk("gva = %#llx, gfn = %#llx", __entry->gva, __entry->gfn)
)

LTTNG_TRACEPOINT_EVENT_INSTANCE(kvm_async_get_page_class, kvm_try_async_get_page,

	TP_PROTO(u64 gva, u64 gfn),

	TP_ARGS(gva, gfn)
)

LTTNG_TRACEPOINT_EVENT_INSTANCE(kvm_async_get_page_class, kvm_async_pf_doublefault,

	TP_PROTO(u64 gva, u64 gfn),

	TP_ARGS(gva, gfn)
)

LTTNG_TRACEPOINT_EVENT_CLASS(kvm_async_pf_nopresent_ready,

	TP_PROTO(u64 token, u64 gva),

	TP_ARGS(token, gva),

	TP_STRUCT__entry(
		__field(__u64, token)
		__field(__u64, gva)
	),

	TP_fast_assign(
		tp_assign(token, token)
		tp_assign(gva, gva)
	),

	TP_printk("token %#llx gva %#llx", __entry->token, __entry->gva)

)

LTTNG_TRACEPOINT_EVENT_INSTANCE(kvm_async_pf_nopresent_ready, kvm_async_pf_not_present,

	TP_PROTO(u64 token, u64 gva),

	TP_ARGS(token, gva)
)

LTTNG_TRACEPOINT_EVENT_INSTANCE(kvm_async_pf_nopresent_ready, kvm_async_pf_ready,

	TP_PROTO(u64 token, u64 gva),

	TP_ARGS(token, gva)
)

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0) \
	|| LTTNG_RHEL_KERNEL_RANGE(3,10,0,7,1, 3,11,0,0,0))

LTTNG_TRACEPOINT_EVENT(
	kvm_async_pf_completed,
	TP_PROTO(unsigned long address, u64 gva),
	TP_ARGS(address, gva),

	TP_STRUCT__entry(
		__field(unsigned long, address)
		__field(u64, gva)
		),

	TP_fast_assign(
		tp_assign(address, address)
		tp_assign(gva, gva)
		),

	TP_printk("gva %#llx address %#lx",  __entry->gva,
		  __entry->address)
)

#else /* #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)) */

LTTNG_TRACEPOINT_EVENT(
	kvm_async_pf_completed,
	TP_PROTO(unsigned long address, struct page *page, u64 gva),
	TP_ARGS(address, page, gva),

	TP_STRUCT__entry(
		__field(unsigned long, address)
		__field(pfn_t, pfn)
		__field(u64, gva)
		),

	TP_fast_assign(
		tp_assign(address, address)
		tp_assign(pfn, page ? page_to_pfn(page) : 0)
		tp_assign(gva, gva)
		),

	TP_printk("gva %#llx address %#lx pfn %#llx",  __entry->gva,
		  __entry->address, __entry->pfn)
)

#endif /* #else #if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)) */

#endif

#endif

#endif /* LTTNG_TRACE_KVM_MAIN_H */

/* This part must be outside protection */
#include "../../../probes/define_trace.h"