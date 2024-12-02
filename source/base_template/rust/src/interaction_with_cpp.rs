

use std::ffi::{c_char, CStr, CString};
use serde::de::DeserializeOwned;

use crate::struct_dto::Error;


pub fn str_to_cchar(text: &str) -> *const c_char {
    CString::new(text).unwrap().into_raw()
}

#[no_mangle]
pub fn cchar_to_string(text: *const c_char) -> String {
    let text_str = unsafe { CStr::from_ptr(text).to_str().unwrap().to_string() };
    text_str
}

fn create_error_object(error_text: &str) -> Error {
    Error {
        _type: String::from("error"),
        text: error_text.to_string(),
    }
}

