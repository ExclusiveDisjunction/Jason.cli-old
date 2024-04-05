
pub trait VarComm {
    type StoredData;

    fn val_eq(&self, other: &Self) -> bool;
    fn get_val(&self) -> &Self::StoredData;
    fn set_val(&mut self, new_data: Self::StoredData);

    fn steralize(&self) -> String;
    fn from_steralize(&mut self, input_string: &str) -> Result<(), String>;
}