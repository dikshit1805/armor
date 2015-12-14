/*
 * lib_ffi.c - FFI library
 *
 * This file is part of ktap by Jovi Zhangwei.
 *
 * Copyright (C) 2012-2013 Jovi Zhangwei <jovi.zhangwei@gmail.com>.
 *
 * ktap is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * ktap is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "../include/ktap_types.h"
#include "../include/ktap_ffi.h"
#include "ktap.h"
#include "kp_vm.h"


static int kplib_ffi_new(ktap_state_t *ks)
{
	int n = kp_arg_nr(ks);
	csymbol_id cs_id = kp_arg_checknumber(ks, 1);
	int array_size = kp_arg_checknumber(ks, 2);
	int is_array = kp_arg_checknumber(ks, 3);
	ktap_cdata_t *cd;

	if (unlikely(n != 3)) {
		/* this is not likely to happen since ffi.new arguments are
		 * generated by compiler */
		set_nil(ks->top++);
		kp_error(ks, "wrong number of arguments\n");
		return 1;
	}

	if (unlikely(cs_id > max_csym_id(ks)))
		kp_error(ks, "invalid csymbol id\n");

	kp_verbose_printf(ks, "ffi.new symbol %s with length %d\n",
			id_to_csym(ks, cs_id)->name, array_size);

	if (is_array)
		cd = kp_cdata_new_ptr(ks, NULL, array_size, cs_id, 1);
	else
		cd = kp_cdata_new_by_id(ks, NULL, cs_id);
	set_cdata(ks->top, cd);
	incr_top(ks);

	return 1;
}

static int kplib_ffi_cast(ktap_state_t *ks)
{
	int n = kp_arg_nr(ks);
	csymbol_id cs_id = kp_arg_checknumber(ks, 1);
	unsigned long addr = kp_arg_checknumber(ks, 2);
	ktap_cdata_t *cd;

	if (unlikely(n != 2)) {
		/* this is not likely to happen since ffi.cast arguments are
		 * generated by compiler */
		set_nil(ks->top++);
		kp_error(ks, "wrong number of arguments\n");
		return 1;
	}

	if (unlikely(cs_id > max_csym_id(ks)))
		kp_error(ks, "invalid csymbol id\n");

	cd = kp_cdata_new_record(ks, (void *)addr, cs_id);
	set_cdata(ks->top, cd);
	incr_top(ks);
	return 1;
}

static int kplib_ffi_sizeof(ktap_state_t *ks)
{
	/*@TODO finish this  08.11 2013 (houqp)*/
	return 0;
}

static const ktap_libfunc_t ffi_lib_funcs[] = {
	{"sizeof", kplib_ffi_sizeof},
	{"new", kplib_ffi_new},
	{"cast", kplib_ffi_cast},
	{NULL}
};

int kp_lib_init_ffi(ktap_state_t *ks)
{
	return kp_vm_register_lib(ks, "ffi", ffi_lib_funcs);
}
