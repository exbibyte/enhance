#[allow(unused_imports)]
#[test]
fn test_scc_kosaraju(){

    use implement::ds::scc_kosaraju;

    let num_nodes = 4usize;
    let rel = vec![ (0,1), (1,2), (2,0), (1,3) ];
    let out = scc_kosaraju::compute( num_nodes, &rel[..] );
    println!( "component map: {:?}", out );
    assert!( out[0] == out[1] && out[1] == out[2] && out[0] != out[3] );

    //todo: add more test cases
}
