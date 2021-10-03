import {MigrationInterface, QueryRunner} from "typeorm";

export class AddCreatedAndModifiedToProject1633178552450 implements MigrationInterface {
    name = 'AddCreatedAndModifiedToProject1633178552450'

    public async up(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`ALTER TABLE "project" ADD "createdAt" TIMESTAMP`);
        await queryRunner.query(`UPDATE "project" SET "createdAt" = now()`);
        await queryRunner.query(`ALTER TABLE "project" ALTER COLUMN "createdAt" SET NOT NULL`);
        await queryRunner.query(`ALTER TABLE "project" ALTER COLUMN "createdAt" SET DEFAULT now()`);

        await queryRunner.query(`ALTER TABLE "project" ADD "modifiedAt" TIMESTAMP`);
        await queryRunner.query(`UPDATE "project" SET "modifiedAt" = now()`);
        await queryRunner.query(`ALTER TABLE "project" ALTER COLUMN "modifiedAt" SET NOT NULL`);
        await queryRunner.query(`ALTER TABLE "project" ALTER COLUMN "modifiedAt" SET DEFAULT now()`);
    }

    public async down(queryRunner: QueryRunner): Promise<void> {
        await queryRunner.query(`ALTER TABLE "project" DROP COLUMN "modifiedAt"`);
        await queryRunner.query(`ALTER TABLE "project" DROP COLUMN "createdAt"`);
    }

}
