#[allow(unused_imports)]
use std::collections::HashSet;
#[test]
fn test_backtrack(){

    use implement::ds::backtrack;

    let input = vec![ 0, 1, 2, 3, 4, 5];
    let mut out = vec![];
    let ans = backtrack::compute( &input[..], & mut out, 0,
                                   & mut | _input: &[i32], index: i32 | vec![index],
                                   & mut | _arr: &[i32], n: i32, total: i32 | n == total );
    let mut hs = HashSet::new();
    for ref i in ans.iter() {
        hs.insert( *i );
    }
    
    assert!( 2isize.pow(input.len() as u32) == ans.len() as isize );
    assert!( hs.len() == ans.len() );
}
