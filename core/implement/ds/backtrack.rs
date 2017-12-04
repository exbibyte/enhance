fn make_move( input: &[i32], out: & mut Vec<i32>, index: i32 ){
    out.push( input[ index as usize ] );
}

fn unmake_move( _input: &[i32], out: & mut Vec<i32>, _index: i32 ){
    out.pop();
}

#[allow(dead_code)]
fn make_candidate( _input: &[i32], index: i32 ) -> Vec<i32> {
    let candidates = vec![index];
    candidates
}

#[allow(dead_code)]
fn is_solution( _arr: &[i32], index: i32, total: i32 ) -> bool {
    index == total
}

pub fn compute< Fcandidate, Fsoln >( arr: & [i32], out: & mut Vec<i32>, n: i32, f_candidate: & mut Fcandidate, f_is_solution: & mut Fsoln ) -> Vec< Vec<i32> > 
    where Fsoln : FnMut( &[i32], i32, i32 ) -> bool,
          Fcandidate: FnMut( &[i32], i32 ) -> Vec<i32>
{
    let mut ans = vec![];
    let total = arr.len();
    // if is_solution( &out[..], n, total as i32 ) {
    if f_is_solution( &out[..], n, total as i32 ) {
        // println!("{:?}", out );
        ans.push( out.clone() );
        return ans
    }else{
        // let candidates = make_candidate( arr, n );
        let candidates = f_candidate( arr, n );
        for i in candidates {
	    make_move( arr, out, i );
            let a = compute(arr, out, n+1, f_candidate, f_is_solution );
            ans.extend_from_slice(&a[..]);
	    unmake_move( arr, out, i );
	    let b = compute(arr,  out, n+1, f_candidate, f_is_solution );
            ans.extend_from_slice(&b[..]);
        }
        return ans
    }
}
