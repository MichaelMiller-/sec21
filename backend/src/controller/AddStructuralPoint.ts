import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Project } from "../entity/Project";
import { StructuralPoint } from "../entity/StructuralPoint"
import Result from "../dto/Result";

export async function addStructuralPoint(request: Request, response: Response) {

    let result = new Result<null>();

    const prj = await getManager().getRepository(Project).findOne(request.params.id);

    if (getManager().getRepository(Project).hasId(prj) === false) {
        result.success = false;
        result.message = "Project not found"
        response.send(result);
        return;
    }

    let pt = new StructuralPoint();
    pt.name = request.body.name;
    pt.coordinate_x = request.body.coordinate_x;
    pt.coordinate_y = request.body.coordinate_y;
    pt.coordinate_z = request.body.coordinate_z;
    pt.project = prj;

    getManager().getRepository(StructuralPoint)
        .save(pt)
        .then(() => {
            result.success = true;
            result.message = "";
        })
        .catch(ex => {
            result.success = false;
            result.message = ex.detail;
        });

    response.send(result);
}
