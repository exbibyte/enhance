use implement::math::sim_anneal;
use std::f32;

#[test]
fn test_sim_anneal(){
    let input = (1..100usize).collect::< Vec< usize > >().iter().map(|&x| x as f32 / 100f32 ).collect::< Vec<f32> >();
    let max_iter = input.len() * 5;
    let initial = input.len()/2;

    let ret = sim_anneal::compute_discrete( &input[..], sim_anneal::candidate_generator_default, sim_anneal::temperature_function_default, sim_anneal::energy_function_default, max_iter, initial );

    let ( expected_index, _ ) = input.iter().enumerate().fold( ( 0usize, f32::MAX ), |acc, (i, &v)| if v < acc.1 { (i,v) } else { acc } );
    // println!( "expected_index: {:?}", expected_index );
    // println!( "ret: {:?}", ret );
    assert!( ret <= expected_index + (input.len() as f32 * 0.02f32) as usize );
}
