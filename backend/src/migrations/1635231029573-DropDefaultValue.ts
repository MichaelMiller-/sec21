import {MigrationInterface, QueryRunner} from "typeorm";

export class DropDefaultValue1635231029573 implements MigrationInterface {
    name = 'DropDefaultValue1635231029573'

    public async up(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`ALTER TABLE "project" ALTER COLUMN "modifiedAt" DROP DEFAULT`);
    }

    public async down(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`ALTER TABLE "project" ALTER COLUMN "modifiedAt" SET DEFAULT now()`);
    }

}
