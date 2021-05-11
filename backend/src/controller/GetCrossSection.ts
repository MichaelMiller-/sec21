import { Request, Response } from "express";
import { getManager } from "typeorm";
import { CrossSection } from "../entity/CrossSection";

export async function getCrossSection(request: Request, response: Response) {
   const values = await getManager().getRepository(CrossSection).findOne(request.params.id);
   response.send(values);
}
