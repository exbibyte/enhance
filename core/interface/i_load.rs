///trait for loading and applying data to a concrete data structure
pub trait ILoad {

    type InputType;

    fn load( & mut self, Self::InputType ) -> Result< (), & 'static str >;

    fn apply( & mut self ) -> Result< bool, & 'static str >;

}
