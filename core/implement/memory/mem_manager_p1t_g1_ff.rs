pub use interface::i_alloc::*;

#[allow(dead_code)]
pub struct MemManagerP1tG1Ff {
    dummy: u32,
}

#[allow(dead_code)]
static mut G_MEM_MANAGER_P1T_G1_FF : MemManagerP1tG1Ff = MemManagerP1tG1Ff { dummy: 10u32 };

#[allow(unused_variables)]
impl IAlloc for MemManagerP1tG1Ff {
    fn newing< T >( & mut self ) -> Result< Box< T >, &'static str > {
        unimplemented!();
    }
    fn freeing< T >( & mut self, p : Box< T > ) -> Result< (), &'static str > {
        unimplemented!();
    }
}
