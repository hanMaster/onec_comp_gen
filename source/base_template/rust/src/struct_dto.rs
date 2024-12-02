use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize)]
pub struct Error {
    #[serde(rename = "Тип")]
    pub _type: String,
    #[serde(rename = "Текст")]
    pub text: String,
}