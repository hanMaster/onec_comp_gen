use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize)]
pub struct Params {
    uuid: String,
    pub name: String,
    #[serde(rename = "type")]
    pub _type: String,
}

#[derive(Serialize, Deserialize)]
pub struct Method {
    uuid: String,
    pub name: String,
    #[serde(rename = "nameEng")]
    pub name_eng: String,
    pub params: Vec<Params>,
    #[serde(rename = "hasReturn")]
    pub has_return: bool,
    #[serde(rename = "ReturnType")]
    return_type: Option<String>,
    #[serde(rename = "callRustMethod")]
    pub call_rust_method: bool,
}

#[derive(Serialize, Deserialize)]
pub struct Props {
    uuid: String,
    name: String,
    #[serde(rename = "canWrite")]
    can_write: bool,
    #[serde(rename = "canRead")]
    can_read: bool,
}

#[derive(Serialize, Deserialize)]
pub struct State {
    pub methods: Vec<Method>,
    pub props: Vec<Props>,
}