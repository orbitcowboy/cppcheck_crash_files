/*
	avr_timer.h

	Copyright 2008, 2009 Michel Pollet <buserror@gmail.com>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __AVR_TIMER_H__
#define __AVR_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
	sim_avr.h

	Copyright 2008-2012 Michel Pollet <buserror@gmail.com>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SIM_AVR_H__
#define __SIM_AVR_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __has_attribute
	#define __has_attribute(x) 0
#endif

#if __has_attribute(fallthrough)
	#define FALLTHROUGH __attribute__((fallthrough));
#else
	#define FALLTHROUGH
#endif


typedef uint32_t avr_flashaddr_t;

struct avr_t;
typedef uint8_t (*avr_io_read_t)(
		struct avr_t * avr,
		avr_io_addr_t addr,
		void * param);
typedef void (*avr_io_write_t)(
		struct avr_t * avr,
		avr_io_addr_t addr,
		uint8_t v,
		void * param);

enum {
	// SREG bit indexes
	S_C = 0,S_Z,S_N,S_V,S_S,S_H,S_T,S_I,

	// 16 bits register pairs
	R_XL	= 0x1a, R_XH,R_YL,R_YH,R_ZL,R_ZH,
	// stack pointer
	R_SPL	= 32+0x3d, R_SPH,
	// real SREG
	R_SREG	= 32+0x3f,

	// maximum number of IO registers, on normal AVRs
	MAX_IOs	= 280,	// Bigger AVRs need more than 256-32 (mega1280)
};

#define AVR_DATA_TO_IO(v) ((v) - 32)
#define AVR_IO_TO_DATA(v) ((v) + 32)

/**
 * Logging macros and associated log levels.
 * The current log level is kept in avr->log.
 */
enum {
	LOG_NONE = 0,
	LOG_OUTPUT,
	LOG_ERROR,
	LOG_WARNING,
	LOG_TRACE,
	LOG_DEBUG,
};


#ifndef AVR_LOG
#define AVR_LOG(avr, level, ...) \
	do { \
		avr_global_logger(avr, level, __VA_ARGS__); \
	} while(0)
#endif
#define AVR_TRACE(avr, ... ) \
	AVR_LOG(avr, LOG_TRACE, __VA_ARGS__)

/*
 * Core states.
 */
enum {
	cpu_Limbo = 0,	// before initialization is finished
	cpu_Stopped,	// all is stopped, timers included

	cpu_Running,	// we're free running

	cpu_Sleeping,	// we're now sleeping until an interrupt

	cpu_Step,		// run ONE instruction, then...
	cpu_StepDone,	// tell gdb it's all OK, and give it registers
	cpu_Done,       // avr software stopped gracefully
	cpu_Crashed,    // avr software crashed (watchdog fired)
};

// this is only ever used if CONFIG_SIMAVR_TRACE is defined
struct avr_trace_data_t {
	struct avr_symbol_t ** codeline;

	/* DEBUG ONLY
	 * this keeps track of "jumps" ie, call,jmp,ret,reti and so on
	 * allows dumping of a meaningful data even if the stack is
	 * munched and so on
	 */
	#define OLD_PC_SIZE	32
	struct {
		uint32_t pc;
		uint16_t sp;
	} old[OLD_PC_SIZE]; // catches reset..
	int			old_pci;

#if AVR_STACK_WATCH
	#define STACK_FRAME_SIZE	32
	// this records the call/ret pairs, to try to catch
	// code that munches the stack -under- their own frame
	struct {
		uint32_t	pc;
		uint16_t 	sp;
	} stack_frame[STACK_FRAME_SIZE];
	int			stack_frame_index;
#endif

	// DEBUG ONLY
	// keeps track of which registers gets touched by instructions
	// reset before each new instructions. Allows meaningful traces
	uint32_t	touched[256 / 32];	// debug
};

typedef void (*avr_run_t)(
		struct avr_t * avr);

#define AVR_FUSE_LOW	0
#define AVR_FUSE_HIGH	1
#define AVR_FUSE_EXT	2

/*
 * Main AVR instance. Some of these fields are set by the AVR "Core" definition files
 * the rest is runtime data (as little as possible)
 */
typedef struct avr_t {
	const char * 		mmcu;	// name of the AVR
	// these are filled by sim_core_declare from constants in /usr/lib/avr/include/avr/io*.h
	uint16_t			ioend;
	uint16_t 			ramend;
	uint32_t			flashend;
	uint32_t			e2end;
	uint8_t				vector_size;
	uint8_t				signature[3];
	uint8_t				fuse[6];
	uint8_t				lockbits;
	avr_io_addr_t		rampz;	// optional, only for ELPM/SPM on >64Kb cores
	avr_io_addr_t		eind;	// optional, only for EIJMP/EICALL on >64Kb cores
	uint8_t				address_size;	// 2, or 3 for cores >128KB in flash
	struct {
		avr_regbit_t		porf;
		avr_regbit_t		extrf;
		avr_regbit_t		borf;
		avr_regbit_t		wdrf;
	} reset_flags;

	// filled by the ELF data, this allow tracking of invalid jumps
	uint32_t			codeend;

	int					state;		// stopped, running, sleeping
	uint32_t			frequency;	// frequency we are running at
	// mostly used by the ADC for now
	uint32_t			vcc,avcc,aref; // (optional) voltages in millivolts

	// cycles gets incremented when sleeping and when running; it corresponds
	// not only to "cycles that runs" but also "cycles that might have run"
	// like, sleeping.
	avr_cycle_count_t	cycle;		// current cycle

	// these next two allow the core to freely run between cycle timers and also allows
	// for a maximum run cycle limit... run_cycle_count is set during cycle timer processing.
	avr_cycle_count_t	run_cycle_count;	// cycles to run before next timer
	avr_cycle_count_t	run_cycle_limit;	// maximum run cycle interval limit

	/**
	 * Sleep requests are accumulated in sleep_usec until the minimum sleep value
	 * is reached, at which point sleep_usec is cleared and the sleep request
	 * is passed on to the operating system.
	 */
	uint32_t 			sleep_usec;
	uint64_t			time_base;	// for avr_get_time_stamp()

	// called at init time
	void (*init)(struct avr_t * avr);
	// called at reset time
	void (*reset)(struct avr_t * avr);

	struct {
		// called at init time (for special purposes like using a
		// memory mapped file as flash see: simduino)
		void (*init)(struct avr_t * avr, void * data);
		// called at termination time ( to clean special initializations)
		void (*deinit)(struct avr_t * avr, void * data);
		// value passed to init() and deinit()
		void *data;
	} custom;

	/*!
	 * Default AVR core run function.
	 * Two modes are available, a "raw" run that goes as fast as
	 * it can, and a "gdb" mode that also watchouts for gdb events
	 * and is a little bit slower.
	 */
	avr_run_t	run;

	/*!
	 * Sleep default behaviour.
	 * In "raw" mode, it calls usleep, in gdb mode, it waits
	 * for howLong for gdb command on it's sockets.
	 */
	void (*sleep)(struct avr_t * avr, avr_cycle_count_t howLong);

	/*!
	 * Every IRQs will be stored in this pool. It is not
	 * mandatory (yet) but will allow listing IRQs and their connections
	 */
	avr_irq_pool_t	irq_pool;

	// Mirror of the SREG register, to facilitate the access to bits
	// in the opcode decoder.
	// This array is re-synthesized back/forth when SREG changes
	uint8_t		sreg[8];

	/* Interrupt state:
		00: idle (no wait, no pending interrupts) or disabled
		<0: wait till zero
		>0: interrupt pending */
	int8_t			interrupt_state;	// interrupt state

	/*
	 * ** current PC **
	 * Note that the PC is representing /bytes/ while the AVR value is
	 * assumed to be "words". This is in line with what GDB does...
	 * this is why you will see >>1 and <<1 in the decoder to handle jumps.
	 * It CAN be a little confusing, so concentrate, young grasshopper.
	 */
	avr_flashaddr_t	pc;
	/*
	 * Reset PC, this is the value used to jump to at reset time, this
	 * allow support for bootloaders
	 */
	avr_flashaddr_t	reset_pc;

	/*
	 * callback when specific IO registers are read/written.
	 * There is one drawback here, there is in way of knowing what is the
	 * "beginning of useful sram" on a core, so there is no way to deduce
	 * what is the maximum IO register for a core, and thus, we can't
	 * allocate this table dynamically.
	 * If you wanted to emulate the BIG AVRs, and XMegas, this would need
	 * work.
	 */
	struct {
		struct avr_irq_t * irq;	// optional, used only if asked for with avr_iomem_getirq()
		struct {
			void * param;
			avr_io_read_t c;
		} r;
		struct {
			void * param;
			avr_io_write_t c;
		} w;
	} io[MAX_IOs];

	/*
	 * This block allows sharing of the IO write/read on addresses between
	 * multiple callbacks. In 99% of case it's not needed, however on the tiny*
	 * (tiny85 at last) some registers have bits that are used by different
	 * IO modules.
	 * If this case is detected, a special "dispatch" callback is installed that
	 * will handle this particular case, without impacting the performance of the
	 * other, normal cases...
	 */
	int				io_shared_io_count;
	struct {
		int used;
		struct {
			void * param;
			void * c;
		} io[4];
	} io_shared_io[4];

	// flash memory (initialized to 0xff, and code loaded into it)
	uint8_t *		flash;
	// this is the general purpose registers, IO registers, and SRAM
	uint8_t *		data;

	// queue of io modules
	struct avr_io_t * io_port;

	// Builtin and user-defined commands
	avr_cmd_table_t commands;
	// cycle timers tracking & delivery
	avr_cycle_timer_pool_t	cycle_timers;
	// interrupt vectors and delivery fifo
	avr_int_table_t	interrupts;

	// DEBUG ONLY -- value ignored if CONFIG_SIMAVR_TRACE = 0
	uint8_t	trace : 1,
			log : 4; // log level, default to 1

	// Only used if CONFIG_SIMAVR_TRACE is defined
	struct avr_trace_data_t *trace_data;

	// VALUE CHANGE DUMP file (waveforms)
	// this is the VCD file that gets allocated if the
	// firmware that is loaded explicitly asks for a trace
	// to be generated, and allocates it's own symbols
	// using AVR_MMCU_TAG_VCD_TRACE (see avr_mcu_section.h)
	struct avr_vcd_t * vcd;

	// gdb hooking structure. Only present when gdb server is active
	struct avr_gdb_t * gdb;

	// if non-zero, the gdb server will be started when the core
	// crashed even if not activated at startup
	// if zero, the simulator will just exit() in case of a crash
	int		gdb_port;

	// buffer for console debugging output from register
	struct {
		char *	 buf;
		uint32_t size;
		uint32_t len;
	} io_console_buffer;
} avr_t;


// this is a static constructor for each of the AVR devices
typedef struct avr_kind_t {
	const char * names[4];	// name aliases
	avr_t * (*make)(void);
} avr_kind_t;

// a symbol loaded from the .elf file
typedef struct avr_symbol_t {
	uint32_t	addr;
	const char  symbol[0];
} avr_symbol_t;

// locate the maker for mcu "name" and allocates a new avr instance
avr_t *
avr_make_mcu_by_name(
		const char *name);
// initializes a new AVR instance. Will call the IO registers init(), and then reset()
int
avr_init(
		avr_t * avr);
// Used by the cores, allocated a mutable avr_t from the const global
avr_t *
avr_core_allocate(
		const avr_t * core,
		uint32_t coreLen);

// resets the AVR, and the IO modules
void
avr_reset(
		avr_t * avr);
// run one cycle of the AVR, sleep if necessary
int
avr_run(
		avr_t * avr);
// finish any pending operations
void
avr_terminate(
		avr_t * avr);

// set an IO register to receive commands from the AVR firmware
// it's optional, and uses the ELF tags
void
avr_set_command_register(
		avr_t * avr,
		avr_io_addr_t addr);

// specify the "console register" -- output sent to this register
// is printed on the simulator console, without using a UART
void
avr_set_console_register(
		avr_t * avr,
		avr_io_addr_t addr);

// load code in the "flash"
void
avr_loadcode(
		avr_t * avr,
		uint8_t * code,
		uint32_t size,
		avr_flashaddr_t address);

/*
 * These are accessors for avr->data but allows watchpoints to be set for gdb
 * IO modules use that to set values to registers, and the AVR core decoder uses
 * that to register "public" read by instructions.
 */
void
avr_core_watch_write(
		avr_t *avr,
		uint16_t addr,
		uint8_t v);
uint8_t
avr_core_watch_read(
		avr_t *avr,
		uint16_t addr);

// called when the core has detected a crash somehow.
// this might activate gdb server
void
avr_sadly_crashed(
		avr_t *avr,
		uint8_t signal);

/*
 * Logs a message using the current logger
 */
void
avr_global_logger(
		struct avr_t* avr,
		const int level,
		const char * format,
		... );

#ifndef AVR_CORE
#include <stdarg.h>
/*
 * Type for custom logging functions
 */
typedef void (*avr_logger_p)(struct avr_t* avr, const int level, const char * format, va_list ap);

/* Sets a global logging function in place of the default */
void
avr_global_logger_set(
		avr_logger_p logger);
/* Gets the current global logger function */
avr_logger_p
avr_global_logger_get(void);
#endif

/*
 * These are callbacks for the two 'main' behaviour in simavr
 */
void avr_callback_sleep_gdb(avr_t * avr, avr_cycle_count_t howLong);
void avr_callback_run_gdb(avr_t * avr);
void avr_callback_sleep_raw(avr_t * avr, avr_cycle_count_t howLong);
void avr_callback_run_raw(avr_t * avr);

/**
 * Accumulates sleep requests (and returns a sleep time of 0) until
 * a minimum count of requested sleep microseconds are reached
 * (low amounts cannot be handled accurately).
 * This function is an utility function for the sleep callbacks
 */
uint32_t
avr_pending_sleep_usec(
		avr_t * avr,
		avr_cycle_count_t howLong);
/* Return the number of 'real time' spent since sim started, in uS */
uint64_t
avr_get_time_stamp(
		avr_t * avr );

#ifdef __cplusplus
};
#endif

#include "sim_io.h"
#include "sim_regbit.h"

#ifdef __GNUC__

# ifndef likely
#  define likely(x) __builtin_expect(!!(x), 1)
# endif

# ifndef unlikely
#  define unlikely(x) __builtin_expect(!!(x), 0)
# endif

#else /* ! __GNUC__ */

# ifndef likely
#  define likely(x) x
# endif

# ifndef unlikely
#  define unlikely(x) x
# endif

#endif /* __GNUC__ */

#endif /*__SIM_AVR_H__*/



enum {
	AVR_TIMER_COMPA = 0,
	AVR_TIMER_COMPB,
	AVR_TIMER_COMPC,

	AVR_TIMER_COMP_COUNT
};

enum {
	TIMER_IRQ_OUT_PWM0 = 0,
	TIMER_IRQ_OUT_PWM1,
	TIMER_IRQ_IN_ICP,	// input capture
	TIMER_IRQ_OUT_COMP,	// comparator pins output IRQ

	TIMER_IRQ_COUNT = TIMER_IRQ_OUT_COMP + AVR_TIMER_COMP_COUNT
};

// Get the internal IRQ corresponding to the INT
#define AVR_IOCTL_TIMER_GETIRQ(_name) AVR_IOCTL_DEF('t','m','r',(_name))

// add timer number/name (character) to set tracing flags
#define AVR_IOCTL_TIMER_SET_TRACE(_number) AVR_IOCTL_DEF('t','m','t',(_number))
// enforce using virtual clock generator when external clock is chosen by firmware
#define AVR_IOCTL_TIMER_SET_VIRTCLK(_number) AVR_IOCTL_DEF('t','m','v',(_number))
// set frequency of the virtual clock generator
#define AVR_IOCTL_TIMER_SET_FREQCLK(_number) AVR_IOCTL_DEF('t','m','f',(_number))

// Waveform generation modes
enum {
	avr_timer_wgm_none = 0,	// invalid mode
	avr_timer_wgm_normal,
	avr_timer_wgm_ctc,
	avr_timer_wgm_pwm,
	avr_timer_wgm_fast_pwm,
	avr_timer_wgm_fc_pwm,
};

// Compare output modes
enum {
	avr_timer_com_normal = 0,// Normal mode, OCnx disconnected
	avr_timer_com_toggle,   // Toggle OCnx on compare match
	avr_timer_com_clear,    // clear OCnx on compare match
	avr_timer_com_set,      // set OCnx on compare match
	
};

enum {
	avr_timer_wgm_reg_constant = 0,
	avr_timer_wgm_reg_ocra,
	avr_timer_wgm_reg_icr,
};

typedef struct avr_timer_wgm_t {
	uint32_t top: 8, bottom: 8, size : 8, kind : 8;
} avr_timer_wgm_t;

#define AVR_TIMER_EXTCLK_CHOOSE 0x80		// marker value for cs_div specifying ext clock selection
#define AVR_TIMER_EXTCLK_FLAG_TN 0x80		// Tn external clock chosen
#define AVR_TIMER_EXTCLK_FLAG_STARTED 0x40	// peripheral started
#define AVR_TIMER_EXTCLK_FLAG_REVDIR 0x20	// reverse counting (decrement)
#define AVR_TIMER_EXTCLK_FLAG_AS2 0x10		// asynchronous external clock chosen
#define AVR_TIMER_EXTCLK_FLAG_VIRT 0x08		// don't use the input pin, generate clock internally
#define AVR_TIMER_EXTCLK_FLAG_EDGE 0x01		// use the rising edge

#define AVR_TIMER_WGM_NORMAL8() { .kind = avr_timer_wgm_normal, .size=8 }
#define AVR_TIMER_WGM_NORMAL16() { .kind = avr_timer_wgm_normal, .size=16 }
#define AVR_TIMER_WGM_CTC() { .kind = avr_timer_wgm_ctc, .top = avr_timer_wgm_reg_ocra }
#define AVR_TIMER_WGM_ICCTC() { .kind = avr_timer_wgm_ctc, .top = avr_timer_wgm_reg_icr }
#define AVR_TIMER_WGM_FASTPWM8() { .kind = avr_timer_wgm_fast_pwm, .size=8 }
#define AVR_TIMER_WGM_FASTPWM9() { .kind = avr_timer_wgm_fast_pwm, .size=9 }
#define AVR_TIMER_WGM_FASTPWM10() { .kind = avr_timer_wgm_fast_pwm, .size=10 }
#define AVR_TIMER_WGM_FCPWM8() { .kind = avr_timer_wgm_fc_pwm, .size=8 }
#define AVR_TIMER_WGM_FCPWM9() { .kind = avr_timer_wgm_fc_pwm, .size=9 }
#define AVR_TIMER_WGM_FCPWM10() { .kind = avr_timer_wgm_fc_pwm, .size=10 }
#define AVR_TIMER_WGM_OCPWM() { .kind = avr_timer_wgm_pwm, .top = avr_timer_wgm_reg_ocra }
#define AVR_TIMER_WGM_ICPWM() { .kind = avr_timer_wgm_pwm, .top = avr_timer_wgm_reg_icr }
#define AVR_TIMER_WGM_ICFASTPWM() { .kind = avr_timer_wgm_fast_pwm, .top = avr_timer_wgm_reg_icr }

typedef struct avr_timer_comp_t {
		avr_int_vector_t	interrupt;		// interrupt vector
		struct avr_timer_t	*timer;			// parent timer
		avr_io_addr_t		r_ocr;			// comparator register low byte
		avr_io_addr_t		r_ocrh;			// comparator register hi byte
		avr_regbit_t		com;			// comparator output mode registers
		avr_regbit_t		com_pin;		// where comparator output is connected
		uint64_t			comp_cycles;
} avr_timer_comp_t, *avr_timer_comp_p;

enum {
	avr_timer_trace_ocr		= (1 << 0),
	avr_timer_trace_tcnt	= (1 << 1),

	avr_timer_trace_compa 	= (1 << 8),
	avr_timer_trace_compb 	= (1 << 9),
	avr_timer_trace_compc 	= (1 << 10),
};

typedef struct avr_timer_t {
	avr_io_t		io;
	char 			name;
	uint32_t		trace;		// debug trace

	avr_regbit_t	disabled;	// bit in the PRR

	avr_io_addr_t	r_tcnt, r_icr;
	avr_io_addr_t	r_tcnth, r_icrh;

	avr_regbit_t	wgm[4];
	avr_timer_wgm_t	wgm_op[16];
	avr_timer_wgm_t	mode;
	int				wgm_op_mode_kind;
	uint32_t		wgm_op_mode_size;

	avr_regbit_t	as2;		// asynchronous clock 32khz
	avr_regbit_t	cs[4];		// specify control register bits choosing clock sourcre
	uint8_t			cs_div[16];	// translate control register value to clock prescaler (orders of 2 exponent)
	uint32_t		cs_div_value;

	avr_regbit_t	ext_clock_pin;	// external clock input pin, to link IRQs
	uint8_t			ext_clock_flags;	// holds AVR_TIMER_EXTCLK_FLAG_ON, AVR_TIMER_EXTCLK_FLAG_EDGE and other ext. clock mode flags
	float			ext_clock;	// external clock frequency, e.g. 32768Hz

	avr_regbit_t	icp;		// input capture pin, to link IRQs
	avr_regbit_t	ices;		// input capture edge select

	avr_timer_comp_t comp[AVR_TIMER_COMP_COUNT];

	avr_int_vector_t overflow;	// overflow
	avr_int_vector_t icr;	// input capture

	uint64_t		tov_cycles;	// number of cycles from zero to overflow
	float			tov_cycles_fract; // fractional part for external clock with non int ratio to F_CPU
	float			phase_accumulator;
	uint64_t		tov_base;	// MCU cycle when the last overflow occured; when clocked externally holds external clock count
	uint16_t		tov_top;	// current top value to calculate tnct
} avr_timer_t;

void avr_timer_init(avr_t * avr, avr_timer_t * port);

#ifdef __cplusplus
};
#endif

#endif /*__AVR_TIMER_H__*/

/*
	avr_adc.h

	Copyright 2008, 2009 Michel Pollet <buserror@gmail.com>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __AVR_ADC_H___
#define __AVR_ADC_H___

#ifdef __cplusplus
extern "C" {
#endif


/*
 * simavr ADC allows external code to feed real voltages to the
 * simulator, and the simulator uses it's 'real' reference voltage
 * to do the right thing and return the 'proper' 10 bits ADC value
 * to the AVR firmware.
 *
 * To send values to the ADC, register your code to wait for the
 * ADC_IRQ_OUT_TRIGGER irq, and at that point send any of the
 * ADC_IRQ_ADC* with Millivolts as value.
 *
 * External trigger is not done yet.
 */

enum {
	// input IRQ values. Values are /always/ volts * 1000 (millivolts)
	ADC_IRQ_ADC0 = 0, ADC_IRQ_ADC1, ADC_IRQ_ADC2, ADC_IRQ_ADC3,
	ADC_IRQ_ADC4, ADC_IRQ_ADC5, ADC_IRQ_ADC6, ADC_IRQ_ADC7,
	ADC_IRQ_ADC8, ADC_IRQ_ADC9, ADC_IRQ_ADC10, ADC_IRQ_ADC11,
	ADC_IRQ_ADC12, ADC_IRQ_ADC13, ADC_IRQ_ADC14, ADC_IRQ_ADC15,
	ADC_IRQ_TEMP,			// see the datasheet
	ADC_IRQ_IN_TRIGGER,
	ADC_IRQ_OUT_TRIGGER,	// sends a avr_adc_mux_t
	ADC_IRQ_COUNT
};

// Get the internal IRQ corresponding to the INT
#define AVR_IOCTL_ADC_GETIRQ AVR_IOCTL_DEF('a','d','c',' ')

/*
 * Definition of a ADC mux mode.
 */
enum {
	ADC_MUX_NONE = 0,		// Nothing. return 0
	ADC_MUX_NOISE,			// Nothing. return something random
	ADC_MUX_SINGLE,			// Normal ADC pin reading
	ADC_MUX_DIFF,			// differential channels (src-diff)
	ADC_MUX_TEMP,			// internal temp sensor
	ADC_MUX_REF,			// reference voltage (in src * 100)
	ADC_MUX_VCC4,			// VCC/4
};
typedef struct avr_adc_mux_t {
	unsigned long kind : 3, gain : 8, diff : 8, src : 13;
} avr_adc_mux_t;

enum {
	ADC_VREF_AREF	= 0,	// default mode
	ADC_VREF_VCC,
	ADC_VREF_AVCC,
	ADC_VREF_V110	= 1100,
	ADC_VREF_V256	= 2560,
};

// ADC trigger sources
typedef enum {
	avr_adts_none = 0,
	avr_adts_free_running,
	avr_adts_analog_comparator_0,
	avr_adts_analog_comparator_1,
	avr_adts_analog_comparator_2,
	avr_adts_analog_comparator_3,
	avr_adts_external_interrupt_0,
	avr_adts_timer_0_compare_match_a,
	avr_adts_timer_0_compare_match_b,
	avr_adts_timer_0_overflow,
	avr_adts_timer_1_compare_match_b,
	avr_adts_timer_1_overflow,
	avr_adts_timer_1_capture_event,
	avr_adts_pin_change_interrupt,
	avr_adts_psc_module_0_sync_signal,
	avr_adts_psc_module_1_sync_signal,
	avr_adts_psc_module_2_sync_signal,
} avr_adts_type;

typedef struct avr_adc_t {
	avr_io_t		io;

	uint8_t 		r_admux;
	// if the last bit exists in the mux, we are an extended ADC
	avr_regbit_t	mux[6];
	avr_regbit_t	ref[3];		// reference voltages bits
	uint16_t		ref_values[7]; // ADC_VREF_*

	avr_regbit_t 	adlar;		// left/right adjustment bit

	uint8_t			r_adcsra;	// ADC Control and Status Register A
	avr_regbit_t 	aden;		// ADC Enabled
	avr_regbit_t 	adsc;		// ADC Start Conversion
	avr_regbit_t 	adate;		// ADC Auto Trigger Enable

	avr_regbit_t	adps[3];	// Prescaler bits. Note that it's a frequency bit shift

	uint8_t			r_adcl, r_adch;	// Data Registers

	uint8_t			r_adcsrb;	// ADC Control and Status Register B
	avr_regbit_t	adts[4];	// Timing Source
	avr_adts_type	adts_op[16];    // ADTS type
	uint8_t		adts_mode;      // the extracted ADTS mode
	avr_regbit_t 	bin;		// Bipolar Input Mode (tinyx5 have it)
	avr_regbit_t 	ipr;		// Input Polarity Reversal (tinyx5 have it)

	// use ADIF and ADIE bits
	avr_int_vector_t adc;

	/*
	 * runtime bits
	 */
	avr_adc_mux_t	muxmode[64];// maximum 6 bits of mux modes
	uint16_t		adc_values[8];	// current values on the ADCs
	uint16_t		temp;		// temp sensor reading
	uint8_t			first;
	uint8_t			read_status;	// marked one when adcl is read
} avr_adc_t;

void avr_adc_init(avr_t * avr, avr_adc_t * port);


/*
 * Helper macros for the Cores definition of muxes
 */
#define AVR_ADC_SINGLE(_chan) { \
		.kind = ADC_MUX_SINGLE, \
		.src = (_chan), \
	}
#define AVR_ADC_DIFF(_a,_b,_g) { \
		.kind = ADC_MUX_DIFF, \
		.src = (_a), \
		.diff = (_b), \
		.gain = (_g), \
	}
#define AVR_ADC_REF(_t) { \
		.kind = ADC_MUX_REF, \
		.src = (_t), \
	}
#define AVR_ADC_TEMP() { \
		.kind = ADC_MUX_TEMP, \
	}

#define AVR_ADC_VCC4() { \
		.kind = ADC_MUX_VCC4, \
	}

#ifdef __cplusplus
};
#endif

#endif /* __AVR_ADC_H___ */


static avr_cycle_count_t
avr_adc_int_raise(
		struct avr_t * avr, avr_cycle_count_t when, void * param)
{
	avr_adc_t * p = (avr_adc_t *)param;
	if (avr_regbit_get(avr, p->aden)) {
		// if the interrupts are not used, still raised the UDRE and TXC flag
		avr_raise_interrupt(avr, &p->adc);
		avr_regbit_clear(avr, p->adsc);
		p->first = 0;
		p->read_status = 0;
		if( p->adts_mode == avr_adts_free_running )
			avr_raise_irq(p->io.irq + ADC_IRQ_IN_TRIGGER, 1);
	}
	return 0;
}

static uint8_t
avr_adc_read_l(
		struct avr_t * avr, avr_io_addr_t addr, void * param)
{
	avr_adc_t * p = (avr_adc_t *)param;

	if (p->read_status)	// conversion already done
		return avr_core_watch_read(avr, addr);

	uint8_t refi = avr_regbit_get_array(avr, p->ref, ARRAY_SIZE(p->ref));
	uint16_t ref = p->ref_values[refi];
	uint8_t muxi = avr_regbit_get_array(avr, p->mux, ARRAY_SIZE(p->mux));
	avr_adc_mux_t mux = p->muxmode[muxi];
	// optional shift left/right
	uint8_t shift = avr_regbit_get(avr, p->adlar) ? 6 : 0; // shift LEFT

	uint32_t reg = 0;
	switch (mux.kind) {
		case ADC_MUX_SINGLE:
			reg = p->adc_values[mux.src];
			break;
		case ADC_MUX_DIFF:
			if (mux.gain == 0)
				mux.gain = 1;
			reg = ((uint32_t)p->adc_values[mux.src] * mux.gain) -
					((uint32_t)p->adc_values[mux.diff] * mux.gain);
			break;
		case ADC_MUX_TEMP:
			reg = p->temp; // assumed to be already calibrated somehow
			break;
		case ADC_MUX_REF:
			reg = mux.src; // reference voltage
			break;
		case ADC_MUX_VCC4:
			if ( !avr->vcc) {
				AVR_LOG(avr, LOG_WARNING, "ADC: missing VCC analog voltage\n");
			} else
				reg = avr->vcc / 4;
			break;
	}
	uint32_t vref = 3300;
	switch (ref) {
		case ADC_VREF_VCC:
			if (!avr->vcc)
				AVR_LOG(avr, LOG_WARNING, "ADC: missing VCC analog voltage\n");
			else
				vref = avr->vcc;
			break;
		case ADC_VREF_AREF:
			if (!avr->aref)
				AVR_LOG(avr, LOG_WARNING, "ADC: missing AREF analog voltage\n");
			else
				vref = avr->aref;
			break;
		case ADC_VREF_AVCC:
			if (!avr->avcc)
				AVR_LOG(avr, LOG_WARNING, "ADC: missing AVCC analog voltage\n");
			else
				vref = avr->avcc;
			break;
		default:
			vref = ref;
	}
//	printf("ADCL %d:%3d:%3d read %4d vref %d:%d=%d\n",
//			mux.kind, mux.diff, mux.src,
//			reg, refi, ref, vref);
	reg = (reg * 0x3ff) / vref;	// scale to 10 bits ADC
//	printf("ADC to 10 bits 0x%x %d\n", reg, reg);
	if (reg > 0x3ff) {
		AVR_LOG(avr, LOG_WARNING, "ADC: channel %d clipped %u/%u VREF %d\n", mux.kind, reg, 0x3ff, vref);
		reg = 0x3ff;
	}
	reg <<= shift;
//	printf("ADC to 10 bits %x shifted %d\n", reg, shift);
	avr->data[p->r_adcl] = reg;
	avr->data[p->r_adch] = reg >> 8;
	p->read_status = 1;
	return avr_core_watch_read(avr, addr);
}

/*
 * From Datasheet:
 * "When ADCL is read, the ADC Data Register is not updated until ADCH is read.
 * Consequently, if the result is left adjusted and no more than 8-bit
 * precision is required, it is sufficient to read ADCH.
 * Otherwise, ADCL must be read first, then ADCH."
 * So here if the H is read before the L, we still call the L to update the
 * register value.
 */
static uint8_t
avr_adc_read_h(
		struct avr_t * avr, avr_io_addr_t addr, void * param)
{
	avr_adc_t * p = (avr_adc_t *)param;
	// no "break" here on purpose
	switch (p->read_status) {
		case 0:
			avr_adc_read_l(avr, p->r_adcl, param);
			FALLTHROUGH
		case 1:
			p->read_status = 2;
			FALLTHROUGH
		default:
			return avr_core_watch_read(avr, addr);
	}
}

static void
avr_adc_configure_trigger(
		struct avr_t * avr, avr_io_addr_t addr, uint8_t v, void * param)
{
	avr_adc_t * p = (avr_adc_t *)param;
	
	uint8_t adate = avr_regbit_get(avr, p->adate);
	uint8_t old_adts = p->adts_mode;
	
	static char * auto_trigger_names[] = {
		"none",
		"free_running",
		"analog_comparator_0",
		"analog_comparator_1",
		"analog_comparator_2",
		"analog_comparator_3",
		"external_interrupt_0",
		"timer_0_compare_match_a",
		"timer_0_compare_match_b",
		"timer_0_overflow",
		"timer_1_compare_match_b",
		"timer_1_overflow",
		"timer_1_capture_event",
		"pin_change_interrupt",
		"psc_module_0_sync_signal",
		"psc_module_1_sync_signal",
		"psc_module_2_sync_signal",
	};
	
	if( adate ) {
		uint8_t adts = avr_regbit_get_array(avr, p->adts, ARRAY_SIZE(p->adts));
		p->adts_mode = p->adts_op[adts];
		
		switch(p->adts_mode) {
			case avr_adts_free_running: {
				// do nothing at free running mode
			}	break;
			// TODO: implement the other auto trigger modes
			default: {
				AVR_LOG(avr, LOG_WARNING,
						"ADC: unimplemented auto trigger mode: %s\n",
						auto_trigger_names[p->adts_mode]);
				p->adts_mode = avr_adts_none;
			}	break;
		}
	} else {
		// TODO: remove previously configured auto triggers
		p->adts_mode = avr_adts_none;
	}
	
	if( old_adts != p->adts_mode )
		AVR_LOG(avr, LOG_TRACE, "ADC: auto trigger configured: %s\n",
				auto_trigger_names[p->adts_mode]);
}

static void
avr_adc_write_adcsra(
		struct avr_t * avr, avr_io_addr_t addr, uint8_t v, void * param)
{
	
	avr_adc_t * p = (avr_adc_t *)param;
	uint8_t adsc = avr_regbit_get(avr, p->adsc);
	uint8_t aden = avr_regbit_get(avr, p->aden);

	avr->data[p->adsc.reg] = v;

	// can't write zero to adsc
	if (adsc && !avr_regbit_get(avr, p->adsc)) {
		avr_regbit_set(avr, p->adsc);
		v = avr->data[p->adsc.reg];
	}
	if (!aden && avr_regbit_get(avr, p->aden)) {
		// first conversion
		p->first = 1;
		AVR_LOG(avr, LOG_TRACE, "ADC: Start AREF %d AVCC %d\n", avr->aref, avr->avcc);
	}
	if (aden && !avr_regbit_get(avr, p->aden)) {
		// stop ADC
		avr_cycle_timer_cancel(avr, avr_adc_int_raise, p);
		avr_regbit_clear(avr, p->adsc);
		v = avr->data[p->adsc.reg];	// Peter Ross pross@xvid.org
	}
	if (!adsc && avr_regbit_get(avr, p->adsc)) {
		// start one!
		uint8_t muxi = avr_regbit_get_array(avr, p->mux, ARRAY_SIZE(p->mux));
		union {
			avr_adc_mux_t mux;
			uint32_t v;
		} e = { .mux = p->muxmode[muxi] };
		avr_raise_irq(p->io.irq + ADC_IRQ_OUT_TRIGGER, e.v);

		// clock prescaler are just a bit shift.. and 0 means 1
		uint32_t div = avr_regbit_get_array(avr, p->adps, ARRAY_SIZE(p->adps));
		if (!div) div++;

		div = avr->frequency >> div;
		if (p->first)
			AVR_LOG(avr, LOG_TRACE, "ADC: starting at %uKHz\n", div / 13 / 100);
		div /= p->first ? 25 : 13;	// first cycle is longer

		avr_cycle_timer_register(avr,
				avr_hz_to_cycles(avr, div),
				avr_adc_int_raise, p);
	}
	avr_core_watch_write(avr, addr, v);
	avr_adc_configure_trigger(avr, addr, v, param);
}

static void
avr_adc_write_adcsrb(
		struct avr_t * avr, avr_io_addr_t addr, uint8_t v, void * param)
{
	avr_core_watch_write(avr, addr, v);
	avr_adc_configure_trigger(avr, addr, v, param);
}

static void
avr_adc_irq_notify(
		struct avr_irq_t * irq, uint32_t value, void * param)
{
	avr_adc_t * p = (avr_adc_t *)param;
	avr_t * avr = p->io.avr;

	switch (irq->irq) {
		case ADC_IRQ_ADC0 ... ADC_IRQ_ADC7: {
			p->adc_values[irq->irq] = value;
		} 	break;
		case ADC_IRQ_TEMP: {
			p->temp = value;
		}	break;
		case ADC_IRQ_IN_TRIGGER: {
			if (avr_regbit_get(avr, p->adate)) {
				// start a conversion only if it's not running
				// otherwise ignore the trigger
				if(!avr_regbit_get(avr, p->adsc) ) {
			  		uint8_t addr = p->adsc.reg;
					if (addr) {
						uint8_t val = avr->data[addr] | (1 << p->adsc.bit);
						// write ADSC to ADCSRA
						avr_adc_write_adcsra(avr, addr, val, param);
					}
				}
			}
		}	break;
	}
}

static void avr_adc_reset(avr_io_t * port)
{
	avr_adc_t * p = (avr_adc_t *)port;

	// stop ADC
	avr_cycle_timer_cancel(p->io.avr, avr_adc_int_raise, p);
	avr_regbit_clear(p->io.avr, p->adsc);

	for (int i = 0; i < ADC_IRQ_COUNT; i++)
		avr_irq_register_notify(p->io.irq + i, avr_adc_irq_notify, p);
}

static const char * irq_names[ADC_IRQ_COUNT] = {
	[ADC_IRQ_ADC0] = "16<adc0",
	[ADC_IRQ_ADC1] = "16<adc1",
	[ADC_IRQ_ADC2] = "16<adc2",
	[ADC_IRQ_ADC3] = "16<adc3",
	[ADC_IRQ_ADC4] = "16<adc4",
	[ADC_IRQ_ADC5] = "16<adc5",
	[ADC_IRQ_ADC6] = "16<adc6",
	[ADC_IRQ_ADC7] = "16<adc7",
	[ADC_IRQ_ADC8] = "16<adc0",
	[ADC_IRQ_ADC9] = "16<adc9",
	[ADC_IRQ_ADC10] = "16<adc10",
	[ADC_IRQ_ADC11] = "16<adc11",
	[ADC_IRQ_ADC12] = "16<adc12",
	[ADC_IRQ_ADC13] = "16<adc13",
	[ADC_IRQ_ADC14] = "16<adc14",
	[ADC_IRQ_ADC15] = "16<adc15",
	[ADC_IRQ_TEMP] = "16<temp",
	[ADC_IRQ_IN_TRIGGER] = "<trigger_in",
	[ADC_IRQ_OUT_TRIGGER] = ">trigger_out",
};

static	avr_io_t	_io = {
	.kind = "adc",
	.reset = avr_adc_reset,
	.irq_names = irq_names,
};

void avr_adc_init(avr_t * avr, avr_adc_t * p)
{
	p->io = _io;

	avr_register_io(avr, &p->io);
	avr_register_vector(avr, &p->adc);
	// allocate this module's IRQ
	avr_io_setirqs(&p->io, AVR_IOCTL_ADC_GETIRQ, ADC_IRQ_COUNT, NULL);

	avr_register_io_write(avr, p->r_adcsra, avr_adc_write_adcsra, p);
	// some ADCs don't have ADCSRB (atmega8/16/32)
	if (p->r_adcsrb)
		avr_register_io_write(avr, p->r_adcsrb, avr_adc_write_adcsrb, p);
	avr_register_io_read(avr, p->r_adcl, avr_adc_read_l, p);
	avr_register_io_read(avr, p->r_adch, avr_adc_read_h, p);
}
