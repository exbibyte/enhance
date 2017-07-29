use implement::render::rendercommon::PipelineArgRequire;
use implement::render::rendercommon::Scene;

pub trait IRenderPass {
    fn process( pipeline_req: PipelineArgRequire, scene: Scene ) -> Result< (), & 'static str >;
}
