import { Request, Response } from "express";
import {DeleteResult, getConnection, getManager} from "typeorm";
import Result from "../dto/Result";
import { Material } from "../entity/Material";



import { Connection, Repository, EntityTarget } from 'typeorm';
import {CurveMember} from "../entity/CurveMember";

class GenericService<T>
{
    private repository: Repository<T>;

    constructor(connection: Connection, repo: EntityTarget<T>) {
        this.repository = connection.getRepository<T>(repo);
    }

    public async delete(id: string): Promise<DeleteResult> {
        return await this.repository.delete(id)
    }

}

function genericDelete<T>(request: Request, response: Response) : Result<null>
{
    let result = new Result<null>();

    const userInteraction = new GenericService<CurveMember>(getConnection(), CurveMember)
    userInteraction.delete(request.params.id)
        .catch((ex) => {
            const msg = ex
            result.success = false
            result.message = msg.detail
        });
    return result
}

export async function deleteMaterial(request: Request, response: Response) {

    let result = new Result<null>();

    await getManager().getRepository(Material)
        .delete(request.params.id)
        .catch((ex) => {
            const msg = ex
            result.success = false
            result.message = msg.detail
        });

    response.send(result);
   // response.send(genericDelete<Material>(request, response));
}
