import { Request, Response } from "express";
import { Any, Equal, getManager, MoreThan, Not } from "typeorm";
import { Project } from "../entity/Project";
import { StructuralPoint } from "../entity/StructuralPoint";
import Result from "../dto/Result";
import { SurfaceMember } from "../entity/SurfaceMember";
import logger from "../logging/Logger";
import { Material } from "../entity/Material";

export async function addSurfaceMember(request: Request, response: Response) {

   let result = new Result<null>();

   const prj = await getManager().getRepository(Project).findOne(request.params.id);
   if (getManager().getRepository(Project).hasId(prj) === false) {
      result.success = false;
      result.message = "Project not found"
      response.send(result);
      return;
   }
   const mat = await getManager().getRepository(Material).findOne(request.params.material);

   let requested_nodes = new Array<number>()
   requested_nodes = request.body.nodes.map(e => { return e })

   if (requested_nodes.length < 4) {
      logger.error("Area cannot be created from less than four Points")
      result.success = false;
      result.message = "Area cannot be created from less than four Points"
      response.send(result);
      return;
   }
   if (requested_nodes[0] != requested_nodes[requested_nodes.length - 1]) {
      result.success = false;
      result.message = "First and last node should be the same (" + requested_nodes[0] + "!=" + requested_nodes[requested_nodes.length - 1] + ")"
      response.send(result);
      return;
   }

   //! \todo constrain to project
   const spts = await getManager().getRepository(StructuralPoint).find({
      id: Any(requested_nodes)
   });

   let nodes = new Array<StructuralPoint>()
   requested_nodes.forEach(nid => {
      const gg = spts.find(e => (e.id === nid));
      nodes.push(gg)
   })

   let obj = new SurfaceMember();
   obj.name = request.body.name;
   obj.project = prj;
   obj.thickness = request.body.thickness
   obj.coordinateX = request.body.coordinateX
   obj.coordinateY = request.body.coordinateY
   obj.coordinateZ = request.body.coordinateZ
   obj.lcsRotation = request.body.lcsRotation
   obj.zAnalysisEccentricity = 0
   obj.material = mat
   obj.structuralPoints = spts
   obj.nodes = nodes

   getManager().getRepository(SurfaceMember)
      .save(obj)
      .then(() => {
         result.success = true;
         result.message = "";
      })
      .catch(ex => {
         console.log(ex)
         result.success = false;
         result.message = ex.detail;
      });

   response.send(result);
}
