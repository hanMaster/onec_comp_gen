mod interaction_with_cpp;
mod impl_test;
//ВставкаМодулей
mod struct_dto;

use interaction_with_cpp::{cchar_to_string, str_to_cchar};
use std::ffi::{c_char, CString};

#[no_mangle]
pub extern "C" fn free_mem_after_cpp(ptr: *mut c_char) -> () {
    unsafe {
        let _ = CString::from_raw(ptr);
    }
}

#[no_mangle]
pub extern "C" fn test__call_from_cpp(addr: *const c_char, fl: f32, bl: bool) -> *const c_char {
    impl_test::main(addr, fl, bl)
}

//ВставкаМетодов