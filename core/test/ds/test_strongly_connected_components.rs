#[allow(unused_imports)]
#[test]
fn test_strongly_connected_components(){

    use implement::ds::strongly_connected_components;

    let nodes = vec![ 0, 1, 2 ,3 ];
    let rel = vec![ (0,1), (1,2), (2,0), (1,3) ];
    let mut out = vec![ 0; nodes.len() ];
    strongly_connected_components::compute( &nodes[..], &rel[..], & mut out[..] );
    println!( "component map: {:?}", out );
    assert!( out[0] == out[1] && out[1] == out[2] && out[0] != out[3] );

    //todo: add more test cases
}
