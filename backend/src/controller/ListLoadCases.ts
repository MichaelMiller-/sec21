import { Request, Response } from "express";
import { getManager } from "typeorm";
import { LoadCase } from "../entity/LoadCase";

export async function listLoadCases(request: Request, response: Response) {

   const values = await getManager()
      .getRepository(LoadCase)
      .createQueryBuilder("loadcase")
      .where("loadcase.project = :id", { id: request.params.id })
      .getMany();

   response.send(values);
}