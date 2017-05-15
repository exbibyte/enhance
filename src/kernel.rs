pub fn () {
    let x = b"Hello World!";
    let color = 0x1f;
    let mut hello_color = [color; 24];
    for( i, char_byte ) in x.into_iter().enumerate(){
        hello_color[i*2] = *char_byte;
    }
    let buffer_ptr = (0xb8000 + 1988) as *mut _;
    unsafe{ *buffer_ptr = hello_color };
    loop{}
}
