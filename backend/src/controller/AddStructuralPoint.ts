import { Request, Response } from "express";
import { getManager } from "typeorm";
import { Project } from "../entity/Project";
import { StructuralPoint } from "../entity/StructuralPoint"

export async function addStructuralPoint(request: Request, response: Response) {

    const prj = await getManager().getRepository(Project).findOne(request.params.id);

    if (getManager().getRepository(Project).hasId(prj) === false) {
        response.send({ "sucess": false, "message": "project not found" });
        return;
    }

    console.log("data: ", request.body);

    // parse json request -> StructuralPoint
    let pt = new StructuralPoint();
    pt.name = request.body.name;
    pt.coordinate_x = request.body.coordinate_x;
    pt.coordinate_y = request.body.coordinate_y;
    pt.coordinate_z = request.body.coordinate_z;
    pt.project = prj;

    // sanity ckechs
    //! \todo cannot the database do the work?
    if (pt.name.length === 0) {
        response.send({ "sucess": false, "message": "name: cannot be null" });
        return;
    }

    const foundPoints = await getManager()
        .getRepository(StructuralPoint)
        .createQueryBuilder("structural_point")
        .where("structural_point.project = :id", { id:  request.params.id })
        .andWhere("structural_point.coordinate_x = :x", { x: pt.coordinate_x })
        .andWhere("structural_point.coordinate_y = :y", { y: pt.coordinate_y })
        .andWhere("structural_point.coordinate_z = :z", { z: pt.coordinate_z })
        .getMany();

    if (foundPoints.length > 0){
        response.send({ "sucess": false, "message": "same position" });
        return;
    }

    //! \todo: exception handling
    getManager().getRepository(StructuralPoint)
        .save(pt)
        .then(pt => { response.send({ "sucess": true, "message": "" }); });
}
