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

    // parse json request -> StructuralPoint
    let pt = new StructuralPoint();
    pt.name = request.body.name;
    pt.coordinate_x = request.body.coordinate_x;
    pt.coordinate_y = request.body.coordinate_y;
    pt.coordinate_z = request.body.coordinate_z;
    pt.project = prj;

    // sanity checks
    //! \todo cannot the database do the work?
    if (pt.name.length === 0) {
        result.success = false;
        result.message = "'name' cannot be empty"
        response.send(result);
        return;
    }

    const foundPoints = await getManager()
        .getRepository(StructuralPoint)
        .createQueryBuilder("structural_point")
        .where("structural_point.project = :id", { id: request.params.id })
        .andWhere("structural_point.coordinate_x = :x", { x: pt.coordinate_x })
        .andWhere("structural_point.coordinate_y = :y", { y: pt.coordinate_y })
        .andWhere("structural_point.coordinate_z = :z", { z: pt.coordinate_z })
        .getMany();

    if (foundPoints.length > 0) {
        result.success = false;
        result.message = "There is already a Structual Point at this position"
        response.send(result);
        return;
    }

    getManager().getRepository(StructuralPoint)
        .save(pt)
        .then(() => {
            result.success = true;
            result.message = "";
        })
        .catch(ex => {
            result.success = false;
            result.message = ex;
        });

    response.send(result);
}
