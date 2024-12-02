use std::ffi::{c_char, CString};
use crate::interaction_with_cpp::{cchar_to_string, str_to_cchar};

//+++Заменить
pub extern "C" fn main(addr: *const c_char, fl: f32, bl: bool) -> *const c_char {
    let part = cchar_to_string(addr) + "+ rust" + &fl.to_string() + &bl.to_string();
    str_to_cchar(&part)
}
//---