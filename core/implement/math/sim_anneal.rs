extern crate rand;

use std::f32;
use self::rand::distributions::{IndependentSample, Range};

pub fn compute_discrete< F0, F1, F2 >( domain: &[ f32 ], candidate_generator: F0, temperature_function: F1, energy_function: F2, max_iter: usize, initial: usize ) -> usize
    where F0: Fn( &[ f32 ], usize ) -> ( usize ), F1: Fn( f32 ) -> f32, F2: Fn( &[ f32 ], usize )-> f32

{
    assert!( initial < domain.len() );

    let between = Range::new(0f32, 1f32);
    let mut rng = rand::thread_rng();

    let mut state = initial; //state is the index within the domain slice

    for i in 0..max_iter {
        // println!( "state: {:?}", state );
        let k = i as f32/ max_iter as f32;
        let t = temperature_function( k ); //output range between [0,1]
        let candidate = candidate_generator( &domain[..], i ); //candidate is the index within the domain slice
        let r = between.ind_sample(&mut rng);
        let e_current = energy_function( &domain[..], state );
        let e_candidate = energy_function( &domain[..], candidate );
        let e_diff = e_candidate - e_current; // diff is between [-1, 1]
        if e_diff < 0f32 { //candidate energy is lower than current
            state = candidate;
        }else{ //diff is between [0, 1]
            let a = t * e_diff;
            if a > r {
                state = candidate;                
            }
        }
    }
    state
}

pub fn temperature_function_default( fraction: f32 ) -> f32 {
    assert!( fraction >= 0f32 && fraction <= 1f32 );
    f32::consts::E.powf( -fraction * 7f32 )
}

pub fn candidate_generator_default( input: &[f32], _index_current: usize ) -> usize {
    let between = Range::new(0usize, input.len() );
    let mut rng = rand::thread_rng();
    between.ind_sample(&mut rng)
}

pub fn energy_function_default( input: &[f32], index_current: usize ) -> f32 {
    let e = input[index_current];
    assert!( e >= 0f32 );
    assert!( e <= 1f32 );
    e
}
