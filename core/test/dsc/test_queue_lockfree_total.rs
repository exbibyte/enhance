
#[test]
fn test_queue(){

    use implement::dsc::queue_lockfree_total::*;
    use interface::*;

    let mut q = QueueLockfreeTotal { _head: 5u32 };
    assert!( q._head == 5u32 );
    assert!( q.size() == 0 );
}
